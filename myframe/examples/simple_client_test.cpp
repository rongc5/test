#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

int main(int argc, char* argv[]) {
    std::string host = "127.0.0.1";
    int port = 8080;
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
    
    std::cout << "简单Socket HTTP客户端测试" << std::endl;
    std::cout << "==========================" << std::endl;
    std::cout << "连接到: " << host << ":" << port << std::endl;
    std::cout << "请求路径: " << path << std::endl;
    
    // 创建socket
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        std::cerr << "创建socket失败" << std::endl;
        return -1;
    }
    
    // 连接服务器
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(host.c_str());
    
    std::cout << "正在连接服务器..." << std::endl;
    if (connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "连接服务器失败" << std::endl;
        close(sock_fd);
        return -1;
    }
    
    std::cout << "连接成功" << std::endl;
    
    // 构造HTTP请求
    std::string request = "GET " + path + " HTTP/1.1\r\n";
    request += "Host: " + host + "\r\n";
    request += "User-Agent: SimpleSocketClient/1.0\r\n";
    request += "Connection: close\r\n";
    request += "\r\n";
    
    std::cout << "\n发送HTTP请求:" << std::endl;
    std::cout << "---请求开始---" << std::endl;
    std::cout << request;
    std::cout << "---请求结束---" << std::endl;
    
    // 发送请求
    ssize_t sent = send(sock_fd, request.c_str(), request.length(), 0);
    if (sent < 0) {
        std::cerr << "发送请求失败" << std::endl;
        close(sock_fd);
        return -1;
    }
    
    std::cout << "已发送 " << sent << " bytes" << std::endl;
    
    // 接收响应
    std::cout << "\n接收HTTP响应:" << std::endl;
    std::cout << "---响应开始---" << std::endl;
    
    char buffer[4096];
    ssize_t total_received = 0;
    
    while (true) {
        ssize_t bytes_read = recv(sock_fd, buffer, sizeof(buffer) - 1, 0);
        if (bytes_read <= 0) {
            break;
        }
        
        buffer[bytes_read] = '\0';
        total_received += bytes_read;
        
        std::cout << buffer;
    }
    
    std::cout << "\n---响应结束---" << std::endl;
    std::cout << "总共接收: " << total_received << " bytes" << std::endl;
    
    close(sock_fd);
    std::cout << "连接已关闭" << std::endl;
    
    return 0;
}