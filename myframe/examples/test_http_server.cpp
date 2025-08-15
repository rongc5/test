#include "../../include/base_net_thread.h"
#include "../../include/listen_thread.h"
#include "../../include/http_req_process.h"
#include "../../include/common_cfgparser.h"
#include <iostream>
#include <signal.h>

// 简单的HTTP数据处理器
class simple_http_data_process : public http_base_data_process
{
public:
    simple_http_data_process(std::shared_ptr<base_net_obj> p) : http_base_data_process(p) {}
    
    virtual void recv_finish() override {
        // 获取请求信息
        http_req_head_para& req = get_req_head_para();
        http_res_head_para& res = get_res_head_para();
        
        std::cout << "收到HTTP请求: " << req._method << " " << req._url_path << std::endl;
        
        // 构造简单的HTTP响应
        std::string response_body = "Hello from MyFrame HTTP Server!\n";
        response_body += "Method: " + req._method + "\n";
        response_body += "Path: " + req._url_path + "\n";
        response_body += "Version: " + req._version + "\n";
        
        // 设置响应头
        res._status_code = 200;
        res._version = "HTTP/1.1";
        res._headers["Content-Type"] = "text/plain";
        res._headers["Content-Length"] = std::to_string(response_body.length());
        res._headers["Connection"] = "close";
        
        // 发送响应
        std::string response = res._version + " 200 OK\r\n";
        for (auto& header : res._headers) {
            response += header.first + ": " + header.second + "\r\n";
        }
        response += "\r\n";
        response += response_body;
        
        // 发送数据
        send_data(response.c_str(), response.length());
    }
    
    virtual size_t process_recv_body(const char *buf, size_t len, int &result) override {
        // 简单处理，直接返回处理完成
        result = 1;
        return len;
    }
};

// HTTP连接处理器
class simple_http_process : public http_req_process
{
public:
    simple_http_process(std::shared_ptr<base_net_obj> p) : http_req_process(p) {
        set_process(new simple_http_data_process(p));
    }
};

// 服务器线程
class http_server_thread : public base_net_thread
{
public:
    http_server_thread() : base_net_thread() {}
    
    virtual std::shared_ptr<base_data_process> get_data_process(std::shared_ptr<base_net_obj> p_net_obj) override {
        return std::make_shared<simple_http_process>(p_net_obj);
    }
};

bool g_running = true;

void signal_handler(int sig) {
    std::cout << "\n收到信号 " << sig << "，准备退出..." << std::endl;
    g_running = false;
}

int main() {
    std::cout << "MyFrame HTTP 服务器测试" << std::endl;
    std::cout << "========================" << std::endl;
    
    // 设置信号处理
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    
    try {
        // 创建服务器线程
        auto server_thread = std::make_shared<http_server_thread>();
        
        // 启动监听
        const int PORT = 8080;
        if (!server_thread->start_listen(PORT)) {
            std::cerr << "启动监听失败，端口: " << PORT << std::endl;
            return -1;
        }
        
        std::cout << "HTTP服务器已启动，监听端口: " << PORT << std::endl;
        std::cout << "测试命令: curl http://localhost:8080/test" << std::endl;
        std::cout << "按Ctrl+C退出" << std::endl;
        
        // 启动线程
        server_thread->start();
        
        // 主循环
        while (g_running) {
            sleep(1);
        }
        
        // 停止服务器
        server_thread->stop();
        std::cout << "HTTP服务器已停止" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "服务器异常: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}