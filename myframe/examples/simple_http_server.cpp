#include "../../include/base_def.h"
#include "../../include/common_def.h"
#include "../../include/http_req_process.h"
#include "../../include/http_base_data_process.h"
#include "../../include/base_connect.h"
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>

// 简单的HTTP数据处理器
class test_http_data_process : public http_base_data_process
{
public:
    test_http_data_process(http_base_process* p) : http_base_data_process(p) {}
    
    virtual void recv_finish() override {
        std::cout << "收到HTTP请求完成" << std::endl;
        
        // 获取请求信息
        http_req_head_para& req = get_req_head_para();
        
        std::cout << "Method: " << req._method << std::endl;
        std::cout << "URL: " << req._url_path << std::endl;
        std::cout << "Version: " << req._version << std::endl;
        
        // 打印请求头
        for (auto& header : req._headers) {
            std::cout << "Header: " << header.first << " = " << header.second << std::endl;
        }
        
        // 构造HTTP响应
        std::string body = "Hello from MyFrame!\n";
        body += "Request Method: " + req._method + "\n";
        body += "Request Path: " + req._url_path + "\n";
        
        std::string response = "HTTP/1.1 200 OK\r\n";
        response += "Content-Type: text/plain\r\n";
        response += "Content-Length: " + std::to_string(body.length()) + "\r\n";
        response += "Connection: close\r\n";
        response += "\r\n";
        response += body;
        
        // 通过底层连接发送响应
        auto net_obj = get_net_obj();
        if (net_obj) {
            net_obj->send_data(response.c_str(), response.length());
        }
        
        std::cout << "已发送HTTP响应" << std::endl;
    }
    
    virtual size_t process_recv_body(const char *buf, size_t len, int &result) override {
        std::cout << "处理HTTP Body: " << len << " bytes" << std::endl;
        result = 1; // 处理完成
        return len;
    }
};

// 简单的HTTP处理器
class test_http_process : public http_req_process
{
public:
    test_http_process(std::shared_ptr<base_net_obj> p) : http_req_process(p) {
        set_process(new test_http_data_process(p));
    }
};

bool g_running = true;

void signal_handler(int sig) {
    std::cout << "\n收到信号，准备退出..." << std::endl;
    g_running = false;
}

int main() {
    std::cout << "MyFrame HTTP服务器简单测试" << std::endl;
    std::cout << "=============================" << std::endl;
    
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
    std::cout << "测试命令: curl http://localhost:8080/test" << std::endl;
    std::cout << "按Ctrl+C退出" << std::endl;
    
    while (g_running) {
        // 接受连接
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        
        if (client_fd < 0) {
            if (g_running) {
                std::cerr << "接受连接失败" << std::endl;
            }
            continue;
        }
        
        std::cout << "接受新连接: " << client_fd << std::endl;
        
        try {
            // 创建网络对象和HTTP处理器
            auto net_obj = std::make_shared<base_connect<test_http_process>>(client_fd);
            auto http_proc = std::make_shared<test_http_process>(net_obj);
            
            // 读取数据
            char buffer[4096];
            ssize_t bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
            
            if (bytes_read > 0) {
                buffer[bytes_read] = '\0';
                std::cout << "收到数据 (" << bytes_read << " bytes):" << std::endl;
                std::cout << buffer << std::endl;
                
                // 处理HTTP请求
                http_proc->process_recv_buf(buffer, bytes_read);
            }
            
        } catch (const std::exception& e) {
            std::cerr << "处理连接异常: " << e.what() << std::endl;
        }
        
        // 关闭客户端连接
        close(client_fd);
        std::cout << "连接已关闭" << std::endl << std::endl;
    }
    
    close(server_fd);
    std::cout << "HTTP服务器已停止" << std::endl;
    
    return 0;
}