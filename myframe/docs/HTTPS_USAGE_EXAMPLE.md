# MyFrame HTTPS 使用示例

本文档展示如何在 MyFrame 中使用新增的 SSL/HTTPS 功能。

## 🔧 编译配置

### 编译不支持SSL的版本 (默认)
```bash
make -f core.mk clean
make -f core.mk ENABLE_SSL=0
# 或直接使用
make -f core.mk
```

### 编译支持SSL的版本
```bash
make -f core.mk clean  
make -f core.mk ENABLE_SSL=1
```

## 📋 前置条件

### 安装 OpenSSL 开发库
```bash
# Ubuntu/Debian
sudo apt-get install libssl-dev

# CentOS/RHEL
sudo yum install openssl-devel
```

### 准备 SSL 证书
```bash
# 生成自签名证书用于测试
openssl req -x509 -newkey rsa:4096 -keyout server.key -out server.crt -days 365 -nodes
```

## 🚀 基本用法

### 1. SSL 服务器示例

```cpp
#include "ssl_context.h"
#include "ssl_connect.h"
#include "http_req_process.h"

class MyHttpProcess; // 你的HTTP处理类

int main() {
    // 初始化SSL全局状态
    ssl_context::init_global();
    
    // 配置SSL
    ssl_config config;
    config._enable = true;
    config._cert_file = "server.crt";
    config._key_file = "server.key";
    config._protocols = "TLSv1.2:TLSv1.3";
    config._verify_peer = false;
    
    // 创建SSL上下文
    ssl_context ssl_ctx;
    if (!ssl_ctx.init_server(config)) {
        std::cerr << "Failed to initialize SSL context" << std::endl;
        return -1;
    }
    
    // 创建监听socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    // ... 绑定端口443等 ...
    
    // 监听连接
    while (true) {
        int client_fd = accept(server_fd, NULL, NULL);
        if (client_fd < 0) continue;
        
        // 创建SSL连接
        auto ssl_conn = std::make_shared<ssl_connect<MyHttpProcess>>(
            client_fd, &ssl_ctx, true);
        
        // 执行SSL握手
        SSL_HANDSHAKE_STATUS status = ssl_conn->ssl_handshake();
        if (status == SSL_HANDSHAKE_DONE) {
            // 握手成功，可以进行HTTPS通信
            // 将连接加入epoll等事件循环
        } else if (status == SSL_HANDSHAKE_WANT_READ || 
                   status == SSL_HANDSHAKE_WANT_WRITE) {
            // 需要更多数据，加入事件循环继续握手
        } else {
            // 握手失败
            ssl_conn->close();
        }
    }
    
    // 清理SSL
    ssl_context::cleanup_global();
    return 0;
}
```

### 2. 事件循环中的SSL握手

```cpp
// 在epoll事件循环中处理SSL握手
void handle_ssl_handshake(std::shared_ptr<ssl_connect<MyHttpProcess>> ssl_conn) {
    SSL_HANDSHAKE_STATUS status = ssl_conn->ssl_handshake();
    
    switch (status) {
        case SSL_HANDSHAKE_DONE:
            // 握手完成，开始正常HTTP处理
            std::cout << "SSL handshake completed" << std::endl;
            // 继续HTTP协议处理
            break;
            
        case SSL_HANDSHAKE_WANT_READ:
            // 需要读取更多数据
            // 保持连接，等待下次可读事件
            break;
            
        case SSL_HANDSHAKE_WANT_WRITE:
            // 需要发送数据
            // 保持连接，等待下次可写事件
            break;
            
        case SSL_HANDSHAKE_ERROR:
        default:
            // 握手失败，关闭连接
            std::cerr << "SSL handshake failed" << std::endl;
            ssl_conn->close();
            break;
    }
}
```

### 3. 配置文件方式

```cpp
// 通过配置文件管理SSL设置
ssl_config load_ssl_config(const std::string& config_file) {
    ssl_config config;
    
    // 使用 common_cfgparser 读取配置
    common_cfgparser parser(config_file.c_str());
    
    config._enable = parser.get_bool("ssl", "enable", false);
    config._cert_file = parser.get_string("ssl", "cert_file", "");
    config._key_file = parser.get_string("ssl", "key_file", "");
    config._protocols = parser.get_string("ssl", "protocols", "TLSv1.2:TLSv1.3");
    config._cipher_list = parser.get_string("ssl", "cipher_list", "HIGH:!aNULL:!MD5");
    config._verify_peer = parser.get_bool("ssl", "verify_peer", false);
    
    return config;
}

// 配置文件示例 (myframe.conf)
/*
[ssl]
enable = true
cert_file = /path/to/server.crt
key_file = /path/to/server.key
protocols = TLSv1.2:TLSv1.3
cipher_list = HIGH:!aNULL:!MD5
verify_peer = false
*/
```

## 🔀 兼容性设计

### 条件编译支持

框架设计为在没有SSL支持时也能正常工作：

```cpp
// 创建连接的通用方法
template<typename PROCESS>
std::shared_ptr<base_connect<PROCESS>> create_connection(
    int fd, ssl_context* ssl_ctx = nullptr, bool use_ssl = false) {
    
#ifdef ENABLE_SSL
    if (use_ssl && ssl_ctx && ssl_ctx->is_initialized()) {
        return std::make_shared<ssl_connect<PROCESS>>(fd, ssl_ctx, true);
    }
#endif
    
    // 回退到普通连接
    return std::make_shared<base_connect<PROCESS>>(fd);
}
```

### 运行时SSL检测

```cpp
bool is_ssl_available() {
#ifdef ENABLE_SSL
    return true;
#else
    return false;
#endif
}

void start_server(bool enable_ssl) {
    if (enable_ssl && !is_ssl_available()) {
        std::cerr << "SSL support not compiled in" << std::endl;
        return;
    }
    
    // 根据SSL支持启动相应的服务器
}
```

## 🛡️ 安全最佳实践

### 1. 证书管理
```cpp
ssl_config secure_config() {
    ssl_config config;
    config._enable = true;
    config._cert_file = "/etc/ssl/certs/server.crt";
    config._key_file = "/etc/ssl/private/server.key";
    config._protocols = "TLSv1.3";  // 只使用最新协议
    config._cipher_list = "ECDHE+AESGCM:ECDHE+CHACHA20:DHE+AESGCM:DHE+CHACHA20:!aNULL:!MD5:!DSS";
    config._verify_peer = true;     // 启用客户端验证
    config._verify_depth = 5;
    return config;
}
```

### 2. 错误处理
```cpp
void handle_ssl_errors(std::shared_ptr<ssl_connect<MyProcess>> conn) {
    std::string error = conn->get_ssl_error_string();
    if (!error.empty() && error != "No SSL error") {
        // 记录详细错误信息
        std::cerr << "SSL Error: " << error << std::endl;
        
        // 关闭连接
        conn->close();
    }
}
```

## 📊 性能考虑

### SSL会话复用
```cpp
// 在ssl_context中启用会话复用 (将来版本)
ssl_context::enable_session_reuse(true);
```

### 内存管理
```cpp
// SSL连接会占用更多内存，注意连接数限制
const int MAX_SSL_CONNECTIONS = 1000;
std::atomic<int> current_ssl_connections{0};

bool can_accept_ssl_connection() {
    return current_ssl_connections.load() < MAX_SSL_CONNECTIONS;
}
```

## 🔧 调试和故障排除

### 1. 编译问题
```bash
# 检查OpenSSL是否安装
pkg-config --exists openssl && echo "OpenSSL available"

# 检查头文件
ls /usr/include/openssl/ssl.h

# 编译时显示详细信息
make -f core.mk ENABLE_SSL=1 VERBOSE=1
```

### 2. 运行时调试
```cpp
// 启用SSL调试输出 (开发时)
#ifdef DEBUG
    SSL_load_error_strings();
    ERR_print_errors_fp(stderr);
#endif
```

### 3. 证书验证
```bash
# 验证证书文件
openssl x509 -in server.crt -text -noout

# 测试SSL连接
openssl s_client -connect localhost:443 -servername localhost
```

## 🎯 使用场景

1. **HTTPS Web服务器** - 处理HTTPS请求
2. **WebSocket over SSL** - WSS协议支持
3. **API服务器** - 安全的REST API
4. **反向代理** - SSL终结和转发

MyFrame现在支持现代Web应用所需的完整SSL/HTTPS功能！🚀✨