#include "../../include/base_def.h"
#include "../../include/common_def.h"
#include "../../include/http_res_process.h"
#include "../../include/out_connect.h"
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

// HTTP响应数据处理器
class test_http_response_data_process : public http_base_data_process
{
public:
    test_http_response_data_process(std::shared_ptr<base_net_obj> p) : http_base_data_process(p) {}
    
    virtual void recv_finish() override {
        std::cout << "收到HTTP响应完成" << std::endl;
        
        // 获取响应信息
        http_res_head_para& res = get_res_head_para();
        
        std::cout << "Status: " << res._status_code << std::endl;
        std::cout << "Version: " << res._version << std::endl;
        
        // 打印响应头
        for (auto& header : res._headers) {
            std::cout << "Header: " << header.first << " = " << header.second << std::endl;
        }
        
        // 打印响应体（如果有）
        if (!_response_body.empty()) {
            std::cout << "Response Body:" << std::endl;
            std::cout << _response_body << std::endl;
        }
    }
    
    virtual size_t process_recv_body(const char *buf, size_t len, int &result) override {
        // 收集响应体
        _response_body.append(buf, len);
        std::cout << "收到响应Body: " << len << " bytes" << std::endl;
        result = 1; // 处理完成
        return len;
    }
    
private:
    std::string _response_body;
};

// HTTP响应处理器
class test_http_response_process : public http_res_process
{
public:
    test_http_response_process(std::shared_ptr<base_net_obj> p) : http_res_process(p) {
        set_process(new test_http_response_data_process(p));
    }
};

int main(int argc, char* argv[]) {
    std::string host = "localhost";
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
    
    std::cout << "MyFrame HTTP客户端测试" << std::endl;
    std::cout << "=======================" << std::endl;
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
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    
    if (inet_pton(AF_INET, host.c_str(), &server_addr.sin_addr) <= 0) {
        // 如果不是IP地址，尝试localhost
        if (host == "localhost") {
            server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        } else {
            std::cerr << "无效的IP地址: " << host << std::endl;
            close(sock_fd);
            return -1;
        }
    }
    
    if (connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "连接服务器失败" << std::endl;
        close(sock_fd);
        return -1;
    }
    
    std::cout << "连接成功" << std::endl;
    
    try {
        // 构造HTTP请求
        std::string request = "GET " + path + " HTTP/1.1\r\n";
        request += "Host: " + host + "\r\n";
        request += "User-Agent: MyFrame-Test-Client/1.0\r\n";
        request += "Connection: close\r\n";
        request += "\r\n";
        
        std::cout << "发送HTTP请求:" << std::endl;
        std::cout << request << std::endl;
        
        // 发送请求
        ssize_t sent = send(sock_fd, request.c_str(), request.length(), 0);
        if (sent < 0) {
            std::cerr << "发送请求失败" << std::endl;
            close(sock_fd);
            return -1;
        }
        
        std::cout << "已发送 " << sent << " bytes" << std::endl;
        
        // 创建网络对象和HTTP响应处理器
        auto net_obj = std::make_shared<base_connect<test_http_response_process>>(sock_fd);
        auto http_proc = std::make_shared<test_http_response_process>(net_obj);
        
        // 接收响应
        char buffer[4096];
        ssize_t total_received = 0;
        
        while (true) {
            ssize_t bytes_read = recv(sock_fd, buffer, sizeof(buffer) - 1, 0);
            if (bytes_read <= 0) {
                break;
            }
            
            buffer[bytes_read] = '\0';
            total_received += bytes_read;
            
            std::cout << "收到数据 (" << bytes_read << " bytes)" << std::endl;
            
            // 处理HTTP响应
            http_proc->process_recv_buf(buffer, bytes_read);
        }
        
        std::cout << "总共接收: " << total_received << " bytes" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "客户端异常: " << e.what() << std::endl;
    }
    
    close(sock_fd);
    std::cout << "连接已关闭" << std::endl;
    
    return 0;
}