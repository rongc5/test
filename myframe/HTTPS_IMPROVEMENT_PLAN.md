# MyFrame HTTPS 改进方案

## 🎯 设计原则

1. **可选集成** - SSL支持作为可选组件，不影响现有HTTP功能
2. **轻量化** - 最小化框架复杂性，支持但不强制使用SSL
3. **兼容性** - 保持现有API接口不变
4. **配置化** - 通过配置开启/关闭SSL功能

## 📋 改进建议

### 1. 基础SSL支持 (推荐)

#### 新增文件结构
```
core/
├── ssl_context.h/.cpp      # SSL上下文管理
├── ssl_connect.h           # SSL连接封装
└── ssl_config.h            # SSL配置管理

include/
├── ssl_context.h
├── ssl_connect.h
└── ssl_config.h
```

#### SSL上下文管理
```cpp
// ssl_context.h
#ifdef ENABLE_SSL
#include <openssl/ssl.h>
#include <openssl/err.h>
#endif

class ssl_context {
public:
    ssl_context();
    ~ssl_context();
    
    bool init_server(const std::string& cert_file, const std::string& key_file);
    bool init_client();
    
#ifdef ENABLE_SSL
    SSL_CTX* get_ssl_ctx() { return _ssl_ctx; }
#endif
    
private:
#ifdef ENABLE_SSL
    SSL_CTX* _ssl_ctx;
#endif
    bool _initialized;
};
```

#### SSL连接封装
```cpp
// ssl_connect.h  
class ssl_connect : public base_connect {
public:
    ssl_connect(int fd, bool is_server = true);
    virtual ~ssl_connect();
    
    virtual bool ssl_handshake();
    virtual int real_recv(char* buf, int len) override;
    virtual int real_send(const char* buf, int len) override;
    
private:
#ifdef ENABLE_SSL
    SSL* _ssl;
#endif
    bool _ssl_handshake_done;
    ssl_context* _ssl_ctx;
};
```

### 2. 编译时配置

#### core.mk 修改
```makefile
# SSL支持配置
ENABLE_SSL ?= 0

ifeq ($(ENABLE_SSL), 1)
    CPPFLAGS += -DENABLE_SSL
    LIB += -lssl -lcrypto
    OBJ += ${SRC_DIR}/ssl_context.o ${SRC_DIR}/ssl_connect.o
endif
```

#### 使用方法
```bash
# 编译不支持SSL的版本 (默认)
make

# 编译支持SSL的版本
make ENABLE_SSL=1
```

### 3. API 保持兼容

#### 现有代码无需修改
```cpp
// 现有HTTP代码保持不变
http_req_process req_proc(net_obj);
```

#### 新增SSL支持
```cpp
// 可选的SSL初始化
#ifdef ENABLE_SSL
ssl_context::init_global();
ssl_context ssl_ctx;
ssl_ctx.init_server("server.crt", "server.key");
#endif

// SSL连接创建
std::shared_ptr<base_net_obj> create_ssl_connection(int fd) {
#ifdef ENABLE_SSL
    return std::make_shared<ssl_connect>(fd, true);
#else
    return std::make_shared<base_connect>(fd);
#endif
}
```

### 4. 配置文件支持

#### 添加SSL配置节
```ini
[ssl]
enable = true
cert_file = /path/to/server.crt
key_file = /path/to/server.key
protocols = TLSv1.2,TLSv1.3
cipher_list = HIGH:!aNULL:!MD5
```

## 🚀 实现优先级

### 高优先级 (必需)
1. **SSL上下文管理** - 基础SSL初始化和配置
2. **SSL连接封装** - 扩展base_connect支持SSL
3. **编译时开关** - 可选编译SSL功能

### 中优先级 (建议)
1. **配置文件集成** - 通过配置文件管理SSL参数
2. **错误处理优化** - SSL特定的错误处理
3. **日志集成** - SSL相关日志 (集成RxLogger)

### 低优先级 (可选)
1. **客户端SSL** - SSL客户端连接支持
2. **证书验证** - 客户端证书验证
3. **SNI支持** - 服务器名称指示支持

## 📦 依赖管理

### OpenSSL 集成
```bash
# 系统安装
sudo apt-get install libssl-dev

# 或编译到libs目录 (与TA-Lib等保持一致)
./scripts/build_openssl_from_source.sh
```

### 版本兼容性
- **OpenSSL 1.1.1+** (推荐)
- **向后兼容** OpenSSL 1.0.2

## 🔧 实现细节

### SSL握手集成
```cpp
// 在base_net_thread中处理SSL握手
void base_net_thread::handle_ssl_handshake(std::shared_ptr<ssl_connect> ssl_conn) {
    if (!ssl_conn->ssl_handshake()) {
        // 握手失败，关闭连接
        ssl_conn->close();
        return;
    }
    
    // 握手成功，继续正常HTTP处理
    continue_http_processing(ssl_conn);
}
```

### 性能考虑
1. **SSL会话复用** - 减少握手开销
2. **异步SSL** - 非阻塞SSL操作
3. **内存池** - SSL缓冲区复用

## 🎛️ 使用示例

### 服务器端
```cpp
#include "ssl_context.h"

// 初始化SSL (仅在启用时)
#ifdef ENABLE_SSL
ssl_context ssl_ctx;
ssl_ctx.init_server("server.crt", "server.key");
#endif

// 创建HTTPS服务器
auto server = create_http_server(443, true); // SSL=true
```

### 客户端
```cpp
// 创建HTTPS客户端连接
auto client = create_http_client("https://example.com", 443);
```

## ✅ 实现收益

1. **安全性提升** - 支持加密通信
2. **功能完整性** - 现代Web服务必需功能
3. **兼容性** - 不影响现有代码
4. **可选性** - 根据需要启用/禁用
5. **标准化** - 遵循OpenSSL标准实现

## 📝 实现建议

鉴于MyFrame是一个轻量级框架，建议采用**渐进式实现**：

1. **第一阶段**: 基础SSL支持 (ssl_context + ssl_connect)
2. **第二阶段**: 配置文件集成和错误处理优化  
3. **第三阶段**: 高级特性 (SNI, 客户端验证等)

这样既保持了框架的轻量化特性，又提供了现代Web应用必需的HTTPS支持。