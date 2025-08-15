#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>
#include <cstring>

#ifdef ENABLE_SSL
#include <openssl/ssl.h>
#include <openssl/err.h>
#endif

bool g_running = true;

void signal_handler(int sig) {
    std::cout << "\n收到信号，准备退出..." << std::endl;
    g_running = false;
}

#ifdef ENABLE_SSL
SSL_CTX* create_ssl_context() {
    const SSL_METHOD *method;
    SSL_CTX *ctx;

    method = TLS_server_method();
    ctx = SSL_CTX_new(method);
    if (!ctx) {
        ERR_print_errors_fp(stderr);
        return nullptr;
    }

    return ctx;
}

bool configure_ssl_context(SSL_CTX *ctx) {
    // 加载证书
    if (SSL_CTX_use_certificate_file(ctx, "test_server.crt", SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        return false;
    }

    // 加载私钥
    if (SSL_CTX_use_PrivateKey_file(ctx, "test_server.key", SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        return false;
    }

    // 验证私钥
    if (!SSL_CTX_check_private_key(ctx)) {
        std::cerr << "私钥和证书不匹配" << std::endl;
        return false;
    }

    return true;
}
#endif

int main() {
    std::cout << "简单Socket HTTPS测试" << std::endl;
    std::cout << "====================" << std::endl;
    
#ifndef ENABLE_SSL
    std::cout << "SSL支持未启用，无法运行HTTPS测试" << std::endl;
    std::cout << "请使用 make ENABLE_SSL=1 重新编译" << std::endl;
    return -1;
#endif

#ifdef ENABLE_SSL
    signal(SIGINT, signal_handler);
    
    // 初始化SSL
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();
    
    SSL_CTX *ctx = create_ssl_context();
    if (!ctx) {
        std::cerr << "创建SSL上下文失败" << std::endl;
        return -1;
    }
    
    if (!configure_ssl_context(ctx)) {
        std::cerr << "配置SSL上下文失败" << std::endl;
        SSL_CTX_free(ctx);
        return -1;
    }
    
    std::cout << "SSL初始化成功" << std::endl;
    
    // 创建socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        std::cerr << "创建socket失败" << std::endl;
        SSL_CTX_free(ctx);
        return -1;
    }
    
    // 设置端口复用
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    // 绑定地址
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8443);
    
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "绑定端口失败" << std::endl;
        close(server_fd);
        SSL_CTX_free(ctx);
        return -1;
    }
    
    // 开始监听
    if (listen(server_fd, 10) < 0) {
        std::cerr << "监听失败" << std::endl;
        close(server_fd);
        SSL_CTX_free(ctx);
        return -1;
    }
    
    std::cout << "HTTPS服务器启动成功，监听端口: 8443" << std::endl;
    std::cout << "测试命令: curl -k https://localhost:8443/" << std::endl;
    std::cout << "或者使用浏览器访问: https://localhost:8443/" << std::endl;
    std::cout << "按Ctrl+C退出" << std::endl;
    
    while (g_running) {
        // 设置超时
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(server_fd, &read_fds);
        
        struct timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;
        
        int activity = select(server_fd + 1, &read_fds, NULL, NULL, &timeout);
        
        if (activity < 0) {
            if (g_running) {
                std::cerr << "select错误" << std::endl;
            }
            break;
        }
        
        if (activity == 0) {
            // 超时，继续循环
            continue;
        }
        
        if (FD_ISSET(server_fd, &read_fds)) {
            // 接受连接
            struct sockaddr_in client_addr;
            socklen_t client_len = sizeof(client_addr);
            int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
            
            if (client_fd < 0) {
                std::cerr << "接受连接失败" << std::endl;
                continue;
            }
            
            std::cout << "\n==== 新HTTPS连接 ====" << std::endl;
            std::cout << "客户端FD: " << client_fd << std::endl;
            
            // 创建SSL连接
            SSL *ssl = SSL_new(ctx);
            SSL_set_fd(ssl, client_fd);
            
            // SSL握手
            int ssl_accept_result = SSL_accept(ssl);
            if (ssl_accept_result <= 0) {
                std::cerr << "SSL握手失败: " << SSL_get_error(ssl, ssl_accept_result) << std::endl;
                ERR_print_errors_fp(stderr);
                SSL_free(ssl);
                close(client_fd);
                continue;
            }
            
            std::cout << "SSL握手成功" << std::endl;
            
            // 读取HTTPS请求
            char buffer[4096];
            int bytes_read = SSL_read(ssl, buffer, sizeof(buffer) - 1);
            
            if (bytes_read > 0) {
                buffer[bytes_read] = '\0';
                std::cout << "收到HTTPS请求 (" << bytes_read << " bytes):" << std::endl;
                std::cout << "---请求开始---" << std::endl;
                std::cout << buffer << std::endl;
                std::cout << "---请求结束---" << std::endl;
                
                // 分析HTTP请求
                std::string request(buffer);
                std::string method, path, version;
                
                size_t space1 = request.find(' ');
                size_t space2 = request.find(' ', space1 + 1);
                size_t newline = request.find('\r', space2 + 1);
                
                if (space1 != std::string::npos && space2 != std::string::npos) {
                    method = request.substr(0, space1);
                    path = request.substr(space1 + 1, space2 - space1 - 1);
                    version = request.substr(space2 + 1, newline - space2 - 1);
                    
                    std::cout << "解析结果:" << std::endl;
                    std::cout << "  Method: " << method << std::endl;
                    std::cout << "  Path: " << path << std::endl;
                    std::cout << "  Version: " << version << std::endl;
                }
                
                // 构造HTTPS响应
                std::string body = "Hello from Simple Socket HTTPS Server!\n";
                body += "Method: " + method + "\n";
                body += "Path: " + path + "\n";
                body += "This is a SSL/TLS encrypted connection test.\n";
                
                std::string response = "HTTP/1.1 200 OK\r\n";
                response += "Content-Type: text/plain\r\n";
                response += "Content-Length: " + std::to_string(body.length()) + "\r\n";
                response += "Connection: close\r\n";
                response += "Server: SimpleSocketHTTPS/1.0\r\n";
                response += "\r\n";
                response += body;
                
                int sent = SSL_write(ssl, response.c_str(), response.length());
                if (sent > 0) {
                    std::cout << "已发送HTTPS响应: " << sent << " bytes" << std::endl;
                } else {
                    std::cerr << "发送响应失败" << std::endl;
                }
            } else {
                std::cout << "没有收到数据或连接关闭" << std::endl;
            }
            
            // 关闭SSL连接
            SSL_shutdown(ssl);
            SSL_free(ssl);
            close(client_fd);
            std::cout << "SSL连接已关闭" << std::endl;
        }
    }
    
    close(server_fd);
    SSL_CTX_free(ctx);
    EVP_cleanup();
    std::cout << "HTTPS服务器已停止" << std::endl;
    
#endif
    return 0;
}