# MyFrame 快速入门指南

本指南将帮助您快速上手 MyFrame 框架。

## 🚀 5分钟快速开始

### 1. 安装依赖

```bash
# Ubuntu/Debian
sudo apt-get update
sudo apt-get install build-essential libssl-dev

# CentOS/RHEL  
sudo yum install gcc-c++ openssl-devel
```

### 2. 克隆和编译

```bash
git clone https://github.com/your-username/MyFrame.git
cd MyFrame

# 基础编译
make -f core.mk

# 或带SSL支持
make -f core.mk ENABLE_SSL=1
```

### 3. 第一个HTTP服务器

创建 `hello_server.cpp`:

```cpp
#include "base_net_thread.h"
#include "http_req_process.h"
#include "http_base_data_process.h"

class hello_data_process : public http_base_data_process {
public:
    hello_data_process(http_base_process* p) : http_base_data_process(p) {}
    
    virtual void msg_recv_finish() override {
        // 构造响应
        std::string body = "Hello, MyFrame World!";
        std::string response = "HTTP/1.1 200 OK\r\n";
        response += "Content-Type: text/plain\r\n";  
        response += "Content-Length: " + std::to_string(body.length()) + "\r\n";
        response += "Connection: close\r\n\r\n";
        response += body;
        
        // 发送响应
        auto net = get_base_net();
        if (net) {
            net->send_data(response.c_str(), response.length());
        }
    }
};

class hello_http_process : public http_req_process {
public:
    hello_http_process(std::shared_ptr<base_net_obj> conn) : http_req_process(conn) {
        set_process(new hello_data_process(this));
    }
};

int main() {
    base_net_thread server;
    
    // 监听8080端口
    if (server.listen<hello_http_process>(8080) != 0) {
        std::cerr << "Failed to listen on port 8080" << std::endl;
        return -1;
    }
    
    std::cout << "HTTP Server started on http://localhost:8080" << std::endl;
    std::cout << "Press Ctrl+C to stop" << std::endl;
    
    // 启动事件循环
    server.run();
    
    return 0;
}
```

### 4. 编译和运行

```bash
# 编译
g++ -std=c++11 -I./include hello_server.cpp -L./core -lmyframe -o hello_server

# 运行
./hello_server
```

### 5. 测试

```bash
# 另一个终端
curl http://localhost:8080
# 输出: Hello, MyFrame World!
```

## 🌟 核心概念

### 线程模型
- **base_net_thread**: 网络线程基类，处理事件循环
- **base_connect**: 连接管理，封装socket操作
- **base_data_process**: 数据处理基类

### 协议处理
- **http_req_process**: HTTP请求处理器
- **web_socket_process**: WebSocket处理器  
- **ssl_connect**: SSL/TLS连接处理

### 消息传递
```cpp
// 创建消息
auto msg = std::make_shared<normal_msg>();
msg->_type = MY_MSG_TYPE;
msg->_data = "Hello";

// 发送消息
thread.send_msg(msg);
```

## 📖 更多示例

### WebSocket 服务器

```cpp
#include "web_socket_res_process.h" 
#include "web_socket_data_process.h"

class my_ws_data_process : public web_socket_data_process {
public:
    virtual void on_message(const std::string& message) override {
        // 回显消息
        send_text_message("Echo: " + message);
    }
};

class my_ws_process : public web_socket_res_process {
public:
    my_ws_process(std::shared_ptr<base_net_obj> conn) : web_socket_res_process(conn) {
        set_process(new my_ws_data_process());
    }
};

int main() {
    base_net_thread server;
    server.listen<my_ws_process>(8080);
    server.run();
}
```

### HTTPS 服务器

```cpp
#include "ssl_context.h"
#include "ssl_connect.h"

int main() {
    // 初始化SSL
    ssl_context::init_global();
    
    ssl_config config;
    config._enable = true;
    config._cert_file = "server.crt";
    config._key_file = "server.key";
    
    ssl_context ssl_ctx;
    ssl_ctx.init_server(config);
    
    base_net_thread server;
    server.listen<ssl_connect<hello_http_process>>(8443, &ssl_ctx);
    server.run();
    
    ssl_context::cleanup_global();
}
```

## 🔧 常用配置

### 编译选项

```bash
# 调试模式
make -f core.mk DEBUG=1

# SSL支持
make -f core.mk ENABLE_SSL=1

# 详细输出
make -f core.mk VERBOSE=1
```

### 运行时配置

```cpp
// 设置线程池大小
server.set_thread_count(4);

// 设置连接超时
server.set_timeout(30000); // 30秒

// 启用keep-alive
server.enable_keepalive(true);
```

## 🐛 常见问题

### 编译错误

**问题**: `fatal error: openssl/ssl.h: No such file or directory`
**解决**: 安装OpenSSL开发库
```bash
sudo apt-get install libssl-dev
```

**问题**: `undefined reference to pthread_create`
**解决**: 添加链接选项
```bash
g++ ... -lpthread
```

### 运行时错误

**问题**: `bind: Address already in use`
**解决**: 端口被占用，换个端口或等待释放
```bash
sudo netstat -tlnp | grep :8080
sudo kill -9 <PID>
```

**问题**: SSL握手失败
**解决**: 检查证书文件路径和权限
```bash
openssl x509 -in server.crt -text -noout
```

## 📚 下一步

- [API参考文档](api.md)
- [HTTPS配置指南](../docs/HTTPS_USAGE_EXAMPLE.md)
- [示例代码](../examples/)
- [性能调优](performance.md)

## 💡 提示

1. **内存管理**: 使用智能指针，避免内存泄漏
2. **错误处理**: 总是检查返回值和异常
3. **性能**: 使用连接池和缓存提升性能
4. **安全**: 在生产环境启用SSL/TLS

开始构建您的高性能网络应用吧！🚀