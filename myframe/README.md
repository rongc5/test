# MyFrame - 高性能C++网络框架

一个轻量级、高性能的现代C++网络框架，专为构建高并发服务器应用而设计。

## ✨ 特性

### 🚀 核心功能
- **高性能网络引擎** - 基于epoll的事件驱动架构
- **多线程支持** - 线程池和异步消息传递
- **HTTP/HTTPS协议** - 完整的HTTP 1.1和HTTPS支持
- **WebSocket/WSS** - 支持WebSocket和WebSocket Secure
- **SSL/TLS加密** - 可选的OpenSSL集成
- **模块化设计** - 高度可扩展的组件架构

### 🛠️ 技术特点
- **现代C++11/14** - 使用智能指针和RAII
- **零拷贝设计** - 高效的内存管理
- **可配置编译** - 支持SSL的条件编译
- **跨平台兼容** - Linux/Unix系统支持
- **轻量级** - 最小化依赖，核心库<100KB

## 📦 快速开始

### 依赖要求

```bash
# 基础依赖
sudo apt-get install build-essential cmake

# SSL支持 (可选)
sudo apt-get install libssl-dev
```

### 编译安装

```bash
# 克隆仓库
git clone https://github.com/your-username/MyFrame.git
cd MyFrame

# 编译基础版本
make -f core.mk

# 或编译SSL版本
make -f core.mk ENABLE_SSL=1
```

### 简单示例

```cpp
#include "base_net_thread.h"
#include "http_req_process.h"

class MyHttpProcess : public http_req_process {
public:
    MyHttpProcess(std::shared_ptr<base_net_obj> conn) 
        : http_req_process(conn) {}
    
    virtual void handle_request() override {
        // 处理HTTP请求
        send_response("Hello, MyFrame!");
    }
};

int main() {
    base_net_thread server;
    server.listen<MyHttpProcess>(8080);
    server.run();
    return 0;
}
```

## 📚 文档

### 基础用法
- [快速入门指南](docs/quickstart.md)
- [API参考文档](docs/api.md)
- [配置选项](docs/configuration.md)

### 高级功能
- [HTTPS/SSL配置](HTTPS_USAGE_EXAMPLE.md)
- [WebSocket支持](docs/websocket.md)
- [自定义协议处理](docs/custom-protocol.md)

### 示例代码
- [HTTP服务器](examples/http_server.cpp)
- [WebSocket聊天室](examples/websocket_chat.cpp)
- [HTTPS API服务](examples/https_api.cpp)

## 🏗️ 架构设计

```
MyFrame 架构
├── core/                   # 核心引擎
│   ├── base_net_thread     # 网络线程基类
│   ├── base_connect        # 连接管理
│   ├── common_epoll        # epoll封装
│   └── ssl_context         # SSL支持
├── protocols/              # 协议实现
│   ├── http_process        # HTTP处理
│   ├── websocket_process   # WebSocket处理
│   └── ssl_connect         # SSL连接
├── utils/                  # 工具库
│   ├── rapidjson/          # JSON解析
│   ├── btree/              # B树实现
│   └── encode/             # 编码工具
└── examples/               # 示例代码
```

## 🔧 编译选项

| 选项 | 描述 | 默认值 |
|------|------|--------|
| `ENABLE_SSL=1` | 启用SSL/TLS支持 | 0 |
| `DEBUG=1` | 启用调试模式 | 0 |
| `VERBOSE=1` | 详细编译输出 | 0 |

## 🚀 性能特点

- **高并发**: 支持10,000+并发连接
- **低延迟**: 微秒级响应时间
- **高吞吐**: 100MB/s+数据传输
- **内存友好**: 智能内存池管理

## 🔒 安全特性

- **TLS 1.2/1.3支持** - 现代加密协议
- **证书验证** - 完整的PKI支持
- **密码套件配置** - 可定制的加密算法
- **HTTPS/WSS** - 全协议栈加密

## 🧪 测试

```bash
# 运行单元测试
make test

# HTTP功能测试
cd test/http_tests
make all
./bin/http_server

# HTTPS功能测试  
make ENABLE_SSL=1
./bin/https_server

# WebSocket测试
./bin/simple_wss_test
```

## 📈 使用场景

- **Web服务器** - 高性能HTTP/HTTPS服务
- **API网关** - RESTful API后端
- **实时通信** - WebSocket聊天、推送
- **微服务** - 轻量级服务间通信
- **IoT平台** - 设备连接和数据收集

## 🤝 贡献

欢迎贡献代码！请遵循以下流程：

1. Fork 本仓库
2. 创建特性分支 (`git checkout -b feature/amazing-feature`)
3. 提交更改 (`git commit -m 'Add amazing feature'`)
4. 推送分支 (`git push origin feature/amazing-feature`)
5. 创建 Pull Request

## 📄 许可证

本项目使用 MIT 许可证 - 查看 [LICENSE](LICENSE) 文件了解详情。

## 🎯 版本规划

### v1.0 (当前)
- ✅ 基础网络框架
- ✅ HTTP/HTTPS支持  
- ✅ WebSocket/WSS支持
- ✅ SSL/TLS集成

### v1.1 (计划中)
- 🔄 HTTP/2 支持
- 🔄 连接池优化
- 🔄 性能监控API
- 🔄 Docker支持

### v2.0 (未来)
- 🔄 gRPC支持
- 🔄 异步文件I/O
- 🔄 分布式追踪
- 🔄 云原生集成

## 📞 联系我们

- **问题反馈**: [GitHub Issues](https://github.com/your-username/MyFrame/issues)
- **功能请求**: [GitHub Discussions](https://github.com/your-username/MyFrame/discussions)
- **安全问题**: security@myframe.org

---

**MyFrame** - 让高性能网络编程变得简单！ 🚀✨