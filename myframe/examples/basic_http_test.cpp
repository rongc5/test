#include "../../include/base_def.h"
#include "../../include/common_def.h"
#include "../../include/http_req_process.h"
#include "../../include/http_base_data_process.h"
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>

// 基础HTTP数据处理器
class basic_http_data_process : public http_base_data_process
{
public:
    basic_http_data_process(http_base_process* p) : http_base_data_process(p) {}
    
    virtual void msg_recv_finish() override {
        std::cout << "HTTP请求接收完成" << std::endl;
        
        // 构造简单响应
        _response_body = "HTTP/1.1 200 OK\r\n";
        _response_body += "Content-Type: text/plain\r\n";
        _response_body += "Content-Length: 28\r\n";
        _response_body += "Connection: close\r\n";
        _response_body += "\r\n";
        _response_body += "Hello from MyFrame Server!";
        
        std::cout << "准备发送响应..." << std::endl;
    }
    
    virtual std::string* get_send_body(int &result) override {
        if (!_response_body.empty()) {
            result = 1; // 发送完成
            return &_response_body;
        }
        result = 1;
        return NULL;
    }
    
    virtual size_t process_recv_body(const char *buf, size_t len, int& result) override {
        std::cout << "收到HTTP Body: " << len << " bytes" << std::endl;
        if (len > 0) {
            std::cout << "Body内容: " << std::string(buf, len) << std::endl;
        }
        result = 1; // 处理完成
        return len;
    }
    
private:
    std::string _response_body;
};

// 基础HTTP处理器
class basic_http_process : public http_req_process
{
public:
    basic_http_process(std::shared_ptr<base_net_obj> p) : http_req_process(p) {
        set_process(new basic_http_data_process(this));
    }
};

bool g_running = true;

void signal_handler(int sig) {
    std::cout << "\n收到信号，准备退出..." << std::endl;
    g_running = false;
}

int main() {
    std::cout << "MyFrame 基础HTTP测试" << std::endl;
    std::cout << "====================" << std::endl;
    
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
    std::cout << "测试命令: curl http://localhost:8080/" << std::endl;
    std::cout << "或者使用浏览器访问: http://localhost:8080/" << std::endl;
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
        
        std::cout << "\n==== 新连接 ====" << std::endl;
        std::cout << "客户端FD: " << client_fd << std::endl;
        
        try {
            // 创建网络对象
            auto net_obj = std::make_shared<base_connect<basic_http_process>>(client_fd);
            
            // 读取HTTP请求
            char buffer[4096];
            ssize_t bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
            
            if (bytes_read > 0) {
                buffer[bytes_read] = '\0';
                std::cout << "收到HTTP请求 (" << bytes_read << " bytes):" << std::endl;
                std::cout << "---请求开始---" << std::endl;
                std::cout << buffer << std::endl;
                std::cout << "---请求结束---" << std::endl;
                
                // 发送简单响应
                std::string response = "HTTP/1.1 200 OK\r\n";
                response += "Content-Type: text/plain\r\n";
                response += "Content-Length: 28\r\n";
                response += "Connection: close\r\n";
                response += "\r\n";
                response += "Hello from MyFrame Server!";
                
                ssize_t sent = send(client_fd, response.c_str(), response.length(), 0);
                if (sent > 0) {
                    std::cout << "已发送响应: " << sent << " bytes" << std::endl;
                } else {
                    std::cerr << "发送响应失败" << std::endl;
                }
            } else {
                std::cout << "没有收到数据或连接关闭" << std::endl;
            }
            
        } catch (const std::exception& e) {
            std::cerr << "处理连接异常: " << e.what() << std::endl;
        }
        
        // 关闭客户端连接
        close(client_fd);
        std::cout << "连接已关闭" << std::endl;
    }
    
    close(server_fd);
    std::cout << "HTTP服务器已停止" << std::endl;
    
    return 0;
}