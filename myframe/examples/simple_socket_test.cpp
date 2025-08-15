#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>
#include <cstring>

bool g_running = true;

void signal_handler(int sig) {
    std::cout << "\n收到信号，准备退出..." << std::endl;
    g_running = false;
}

int main() {
    std::cout << "简单Socket HTTP测试" << std::endl;
    std::cout << "===================" << std::endl;
    
    signal(SIGINT, signal_handler);
    
    // 创建socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        std::cerr << "创建socket失败" << std::endl;
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
    address.sin_port = htons(8080);
    
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "绑定端口失败" << std::endl;
        close(server_fd);
        return -1;
    }
    
    // 开始监听
    if (listen(server_fd, 10) < 0) {
        std::cerr << "监听失败" << std::endl;
        close(server_fd);
        return -1;
    }
    
    std::cout << "HTTP服务器启动成功，监听端口: 8080" << std::endl;
    std::cout << "测试命令: curl http://localhost:8080/" << std::endl;
    std::cout << "或者使用浏览器访问: http://localhost:8080/" << std::endl;
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
            
            std::cout << "\n==== 新HTTP连接 ====" << std::endl;
            std::cout << "客户端FD: " << client_fd << std::endl;
            
            // 读取HTTP请求
            char buffer[4096];
            ssize_t bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
            
            if (bytes_read > 0) {
                buffer[bytes_read] = '\0';
                std::cout << "收到HTTP请求 (" << bytes_read << " bytes):" << std::endl;
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
                
                // 构造HTTP响应
                std::string body = "Hello from Simple Socket HTTP Server!\n";
                body += "Method: " + method + "\n";
                body += "Path: " + path + "\n";
                body += "This is a basic HTTP implementation test.\n";
                
                std::string response = "HTTP/1.1 200 OK\r\n";
                response += "Content-Type: text/plain\r\n";
                response += "Content-Length: " + std::to_string(body.length()) + "\r\n";
                response += "Connection: close\r\n";
                response += "Server: SimpleSocketTest/1.0\r\n";
                response += "\r\n";
                response += body;
                
                ssize_t sent = send(client_fd, response.c_str(), response.length(), 0);
                if (sent > 0) {
                    std::cout << "已发送HTTP响应: " << sent << " bytes" << std::endl;
                } else {
                    std::cerr << "发送响应失败" << std::endl;
                }
            } else {
                std::cout << "没有收到数据或连接关闭" << std::endl;
            }
            
            // 关闭客户端连接
            close(client_fd);
            std::cout << "连接已关闭" << std::endl;
        }
    }
    
    close(server_fd);
    std::cout << "HTTP服务器已停止" << std::endl;
    
    return 0;
}