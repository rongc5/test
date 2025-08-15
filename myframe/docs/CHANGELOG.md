# MyFrame 更新日志

所有重要的项目变更都将记录在此文件中。

格式遵循 [Keep a Changelog](https://keepachangelog.com/zh-CN/1.0.0/)，
版本号遵循 [语义化版本](https://semver.org/lang/zh-CN/)。

## [Unreleased]

## [1.0.0] - 2025-08-15

### 新增 ✨
- **SSL/TLS 支持** - 完整的 HTTPS 和 WSS 功能
  - 添加 `ssl_context` 和 `ssl_connect` 类
  - 支持 TLS 1.2 和 TLS 1.3 协议
  - 可配置的密码套件和证书验证
  - 条件编译支持 (`ENABLE_SSL=1`)

- **WebSocket Secure (WSS)** - 加密的 WebSocket 连接
  - SSL/TLS 上的 WebSocket 协议
  - 完整的握手和帧处理
  - 支持标准 WebSocket 客户端

- **HTTP/HTTPS 功能增强**
  - 完整的 HTTP 1.1 协议支持
  - HTTPS 服务器和客户端实现
  - 请求/响应处理优化

- **测试套件** - 全面的功能验证
  - HTTP 服务器/客户端测试
  - HTTPS 加密通信测试  
  - WSS 双向通信测试
  - 与标准工具 (curl) 兼容性验证

### 改进 🚀
- **日志系统分离** - 将日志功能分离到独立的 RxLogger 库
  - 移除框架内置的日志代码
  - 减少核心库体积和复杂度
  - 提供外部日志库集成接口

- **编译系统优化**
  - 支持可选的 SSL 编译
  - 改进的 Makefile 结构
  - 更好的依赖管理

- **代码质量提升**
  - 移除所有日志相关的宏和函数调用
  - 改进错误处理机制
  - 增强内存管理

### 移除 🗑️
- **内置日志系统** - 以下文件已移除：
  - `core/log_base.cpp`
  - `core/log_base.h`
  - `core/log_helper.h`
  - `core/log_process.cpp`
  - `core/log_process.h`
  - `core/log_thread.cpp`
  - `core/log_thread.h`
  - `test/test_log.cpp`

### 修复 🐛
- 修复多线程环境下的内存泄漏问题
- 改进 SSL 握手的错误处理
- 修复 WebSocket 帧解析的边界情况

### 安全 🔒
- **加密通信** - 添加完整的 TLS 支持
- **证书验证** - 可配置的证书验证策略
- **安全密码套件** - 默认使用现代加密算法

## [0.9.0] - 历史版本

### 新增
- 基础网络框架核心功能
- HTTP 协议基础支持
- WebSocket 协议实现
- epoll 事件循环
- 多线程架构

---

## 版本说明

### 语义化版本规则
- **主版本号**: 不兼容的 API 修改
- **次版本号**: 向后兼容的功能性新增
- **修订版本号**: 向后兼容的问题修正

### 更新类型
- **新增** ✨ - 新功能
- **改进** 🚀 - 现有功能的改进
- **修复** 🐛 - 问题修复
- **移除** 🗑️ - 移除的功能
- **安全** 🔒 - 安全相关更新
- **废弃** ⚠️ - 即将移除的功能