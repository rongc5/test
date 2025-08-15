#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

#ifdef ENABLE_SSL
#include <openssl/ssl.h>
#include <openssl/err.h>
#endif

#ifdef ENABLE_SSL
bool init_ssl_client() {
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();
    return true;
}

SSL_CTX* create_client_ssl_context() {
    const SSL_METHOD *method;
    SSL_CTX *ctx;

    method = TLS_client_method();
    ctx = SSL_CTX_new(method);
    if (!ctx) {
        ERR_print_errors_fp(stderr);
        return nullptr;
    }

    // 设置验证模式（忽略证书验证用于测试）
    SSL_CTX_set_verify(ctx, SSL_VERIFY_NONE, NULL);

    return ctx;
}
#endif

int main(int argc, char* argv[]) {
    std::string host = "127.0.0.1";
    int port = 8443;
    std::string path = "/test";
    
    if (argc > 1) {
        host = argv[1];
    }
    if (argc > 2) {
        port = std::atoi(argv[2]);
    }
    if (argc > 3) {
        path = argv[3];
    }
    
    std::cout << "简单Socket HTTPS客户端测试" << std::endl;
    std::cout << "===========================" << std::endl;
    
#ifndef ENABLE_SSL
    std::cout << "SSL支持未启用，无法运行HTTPS测试" << std::endl;
    std::cout << "请使用 make ENABLE_SSL=1 重新编译" << std::endl;
    return -1;
#endif

#ifdef ENABLE_SSL
    std::cout << "连接到: " << host << ":" << port << std::endl;
    std::cout << "请求路径: " << path << std::endl;
    
    // 初始化SSL
    if (!init_ssl_client()) {
        std::cerr << "初始化SSL客户端失败" << std::endl;
        return -1;
    }
    
    SSL_CTX *ctx = create_client_ssl_context();
    if (!ctx) {
        std::cerr << "创建SSL上下文失败" << std::endl;
        return -1;
    }
    
    std::cout << "SSL客户端初始化成功" << std::endl;
    
    // 创建socket
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        std::cerr << "创建socket失败" << std::endl;
        SSL_CTX_free(ctx);
        return -1;
    }
    
    // 连接服务器
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(host.c_str());
    
    std::cout << "正在连接HTTPS服务器..." << std::endl;
    if (connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "连接服务器失败" << std::endl;
        close(sock_fd);
        SSL_CTX_free(ctx);
        return -1;
    }
    
    std::cout << "TCP连接成功" << std::endl;
    
    // 创建SSL连接
    SSL *ssl = SSL_new(ctx);
    SSL_set_fd(ssl, sock_fd);
    
    // SSL握手
    std::cout << "开始SSL握手..." << std::endl;
    int ssl_connect_result = SSL_connect(ssl);
    if (ssl_connect_result <= 0) {
        std::cerr << "SSL握手失败: " << SSL_get_error(ssl, ssl_connect_result) << std::endl;
        ERR_print_errors_fp(stderr);
        SSL_free(ssl);
        close(sock_fd);
        SSL_CTX_free(ctx);
        return -1;
    }
    
    std::cout << "SSL握手成功" << std::endl;
    
    // 构造HTTPS请求
    std::string request = "GET " + path + " HTTP/1.1\r\n";
    request += "Host: " + host + "\r\n";
    request += "User-Agent: SimpleSocketHTTPSClient/1.0\r\n";
    request += "Connection: close\r\n";
    request += "\r\n";
    
    std::cout << "\n发送HTTPS请求:" << std::endl;
    std::cout << "---请求开始---" << std::endl;
    std::cout << request;
    std::cout << "---请求结束---" << std::endl;
    
    // 发送请求
    int sent = SSL_write(ssl, request.c_str(), request.length());
    if (sent < 0) {
        std::cerr << "发送请求失败" << std::endl;
        SSL_free(ssl);
        close(sock_fd);
        SSL_CTX_free(ctx);
        return -1;
    }
    
    std::cout << "已发送 " << sent << " bytes" << std::endl;
    
    // 接收响应
    std::cout << "\n接收HTTPS响应:" << std::endl;
    std::cout << "---响应开始---" << std::endl;
    
    char buffer[4096];
    int total_received = 0;
    
    while (true) {
        int bytes_read = SSL_read(ssl, buffer, sizeof(buffer) - 1);
        if (bytes_read <= 0) {
            break;
        }
        
        buffer[bytes_read] = '\0';
        total_received += bytes_read;
        
        std::cout << buffer;
    }
    
    std::cout << "\n---响应结束---" << std::endl;
    std::cout << "总共接收: " << total_received << " bytes" << std::endl;
    
    // 关闭SSL连接
    SSL_shutdown(ssl);
    SSL_free(ssl);
    close(sock_fd);
    SSL_CTX_free(ctx);
    EVP_cleanup();
    
    std::cout << "HTTPS连接已关闭" << std::endl;
    
#endif
    return 0;
}