#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <sstream>
#include <iomanip>

#ifdef ENABLE_SSL
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#endif

const std::string WS_GUID = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

#ifdef ENABLE_SSL

// Base64编码函数
std::string base64_encode(const std::string& input) {
    BIO *bio, *b64;
    BUF_MEM *bufferPtr;

    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new(BIO_s_mem());
    bio = BIO_push(b64, bio);

    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
    BIO_write(bio, input.c_str(), input.length());
    BIO_flush(bio);
    BIO_get_mem_ptr(bio, &bufferPtr);

    std::string result(bufferPtr->data, bufferPtr->length);
    BIO_free_all(bio);

    return result;
}

// 生成WebSocket Accept Key
std::string generate_accept_key(const std::string& client_key) {
    std::string combined = client_key + WS_GUID;
    
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1(reinterpret_cast<const unsigned char*>(combined.c_str()), combined.length(), hash);
    
    std::string hash_str(reinterpret_cast<char*>(hash), SHA_DIGEST_LENGTH);
    return base64_encode(hash_str);
}

// SSL初始化函数
bool init_ssl() {
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();
    return true;
}

SSL_CTX* create_server_ssl_context() {
    const SSL_METHOD *method = TLS_server_method();
    SSL_CTX *ctx = SSL_CTX_new(method);
    if (!ctx) {
        ERR_print_errors_fp(stderr);
        return nullptr;
    }

    // 加载证书
    if (SSL_CTX_use_certificate_file(ctx, "test_server.crt", SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        SSL_CTX_free(ctx);
        return nullptr;
    }

    // 加载私钥
    if (SSL_CTX_use_PrivateKey_file(ctx, "test_server.key", SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        SSL_CTX_free(ctx);
        return nullptr;
    }

    return ctx;
}

SSL_CTX* create_client_ssl_context() {
    const SSL_METHOD *method = TLS_client_method();
    SSL_CTX *ctx = SSL_CTX_new(method);
    if (!ctx) {
        ERR_print_errors_fp(stderr);
        return nullptr;
    }

    SSL_CTX_set_verify(ctx, SSL_VERIFY_NONE, NULL);
    return ctx;
}

// WSS服务器
void run_wss_server() {
    std::cout << "=== WSS 服务器测试 ===" << std::endl;
    
    if (!init_ssl()) {
        std::cerr << "SSL初始化失败" << std::endl;
        return;
    }

    SSL_CTX *ctx = create_server_ssl_context();
    if (!ctx) {
        std::cerr << "SSL上下文创建失败" << std::endl;
        return;
    }

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8444);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "绑定失败" << std::endl;
        close(server_fd);
        SSL_CTX_free(ctx);
        return;
    }

    if (listen(server_fd, 10) < 0) {
        std::cerr << "监听失败" << std::endl;
        close(server_fd);
        SSL_CTX_free(ctx);
        return;
    }

    std::cout << "WSS服务器启动，监听端口: 8444" << std::endl;
    std::cout << "等待WebSocket over SSL连接..." << std::endl;

    while (true) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);

        if (client_fd < 0) {
            std::cerr << "接受连接失败" << std::endl;
            continue;
        }

        std::cout << "\n新SSL连接" << std::endl;

        // SSL握手
        SSL *ssl = SSL_new(ctx);
        SSL_set_fd(ssl, client_fd);

        if (SSL_accept(ssl) <= 0) {
            std::cerr << "SSL握手失败" << std::endl;
            ERR_print_errors_fp(stderr);
            SSL_free(ssl);
            close(client_fd);
            continue;
        }

        std::cout << "SSL握手成功" << std::endl;

        // 读取WebSocket握手请求
        char buffer[4096];
        int bytes_read = SSL_read(ssl, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            std::cout << "收到WebSocket握手请求:" << std::endl;
            std::cout << buffer << std::endl;

            // 解析WebSocket Key
            std::string request(buffer);
            size_t key_pos = request.find("Sec-WebSocket-Key: ");
            if (key_pos != std::string::npos) {
                key_pos += 19; // "Sec-WebSocket-Key: "的长度
                size_t key_end = request.find("\r\n", key_pos);
                std::string ws_key = request.substr(key_pos, key_end - key_pos);
                
                std::cout << "WebSocket Key: " << ws_key << std::endl;

                // 生成Accept Key
                std::string accept_key = generate_accept_key(ws_key);
                std::cout << "Accept Key: " << accept_key << std::endl;

                // 构造WebSocket握手响应
                std::string response = "HTTP/1.1 101 Switching Protocols\r\n";
                response += "Upgrade: websocket\r\n";
                response += "Connection: Upgrade\r\n";
                response += "Sec-WebSocket-Accept: " + accept_key + "\r\n";
                response += "\r\n";

                // 发送响应
                int sent = SSL_write(ssl, response.c_str(), response.length());
                if (sent > 0) {
                    std::cout << "WebSocket握手响应已发送" << std::endl;
                    std::cout << "WSS连接建立成功！" << std::endl;

                    // 发送一个WebSocket文本消息
                    std::string message = "Hello from WSS Server!";
                    
                    // WebSocket帧格式：FIN(1) + RSV(3) + Opcode(4) + MASK(1) + Payload len(7)
                    std::string frame;
                    frame.push_back(0x81); // FIN=1, Opcode=1 (text)
                    frame.push_back(message.length()); // 负载长度
                    frame += message;

                    SSL_write(ssl, frame.c_str(), frame.length());
                    std::cout << "已发送WebSocket消息: " << message << std::endl;

                } else {
                    std::cerr << "发送WebSocket响应失败" << std::endl;
                }
            }
        }

        SSL_shutdown(ssl);
        SSL_free(ssl);
        close(client_fd);
        std::cout << "WSS连接已关闭\n" << std::endl;
        break; // 只处理一个连接用于测试
    }

    close(server_fd);
    SSL_CTX_free(ctx);
    EVP_cleanup();
}

// WSS客户端
void run_wss_client() {
    std::cout << "=== WSS 客户端测试 ===" << std::endl;
    
    if (!init_ssl()) {
        std::cerr << "SSL初始化失败" << std::endl;
        return;
    }

    SSL_CTX *ctx = create_client_ssl_context();
    if (!ctx) {
        std::cerr << "SSL上下文创建失败" << std::endl;
        return;
    }

    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8444);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    std::cout << "连接到WSS服务器..." << std::endl;
    if (connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "连接失败" << std::endl;
        close(sock_fd);
        SSL_CTX_free(ctx);
        return;
    }

    // SSL握手
    SSL *ssl = SSL_new(ctx);
    SSL_set_fd(ssl, sock_fd);

    if (SSL_connect(ssl) <= 0) {
        std::cerr << "SSL握手失败" << std::endl;
        ERR_print_errors_fp(stderr);
        SSL_free(ssl);
        close(sock_fd);
        SSL_CTX_free(ctx);
        return;
    }

    std::cout << "SSL连接建立成功" << std::endl;

    // 发送WebSocket握手请求
    std::string ws_key = "dGhlIHNhbXBsZSBub25jZQ=="; // 示例key
    std::string request = "GET / HTTP/1.1\r\n";
    request += "Host: localhost:8444\r\n";
    request += "Upgrade: websocket\r\n";
    request += "Connection: Upgrade\r\n";
    request += "Sec-WebSocket-Key: " + ws_key + "\r\n";
    request += "Sec-WebSocket-Version: 13\r\n";
    request += "\r\n";

    std::cout << "发送WebSocket握手请求..." << std::endl;
    SSL_write(ssl, request.c_str(), request.length());

    // 读取响应
    char buffer[4096];
    int bytes_read = SSL_read(ssl, buffer, sizeof(buffer) - 1);
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0';
        std::cout << "收到WebSocket握手响应:" << std::endl;
        std::cout << buffer << std::endl;

        if (strstr(buffer, "101 Switching Protocols")) {
            std::cout << "WebSocket握手成功！" << std::endl;

            // 读取WebSocket消息
            bytes_read = SSL_read(ssl, buffer, sizeof(buffer) - 1);
            if (bytes_read > 0) {
                // 解析WebSocket帧
                if (bytes_read >= 2) {
                    uint8_t fin = (buffer[0] & 0x80) >> 7;
                    uint8_t opcode = buffer[0] & 0x0F;
                    uint8_t masked = (buffer[1] & 0x80) >> 7;
                    uint8_t payload_len = buffer[1] & 0x7F;

                    std::cout << "收到WebSocket帧:" << std::endl;
                    std::cout << "  FIN: " << (int)fin << std::endl;
                    std::cout << "  Opcode: " << (int)opcode << std::endl;
                    std::cout << "  Masked: " << (int)masked << std::endl;
                    std::cout << "  Payload Length: " << (int)payload_len << std::endl;

                    if (opcode == 1 && payload_len > 0 && bytes_read >= 2 + payload_len) {
                        std::string message(buffer + 2, payload_len);
                        std::cout << "  消息内容: " << message << std::endl;
                    }
                }
            }
        } else {
            std::cout << "WebSocket握手失败" << std::endl;
        }
    }

    SSL_shutdown(ssl);
    SSL_free(ssl);
    close(sock_fd);
    SSL_CTX_free(ctx);
    EVP_cleanup();
}

#endif

int main(int argc, char* argv[]) {
    std::cout << "WebSocket over SSL (WSS) 测试" << std::endl;
    std::cout << "==============================" << std::endl;

#ifndef ENABLE_SSL
    std::cout << "SSL支持未启用，无法运行WSS测试" << std::endl;
    std::cout << "请使用 make ENABLE_SSL=1 重新编译" << std::endl;
    return -1;
#endif

#ifdef ENABLE_SSL
    std::string mode = "server";
    if (argc > 1) {
        mode = argv[1];
    }

    if (mode == "server") {
        run_wss_server();
    } else if (mode == "client") {
        run_wss_client();
    } else {
        std::cout << "用法: " << argv[0] << " [server|client]" << std::endl;
        std::cout << "默认运行服务器模式" << std::endl;
        run_wss_server();
    }
#endif

    return 0;
}