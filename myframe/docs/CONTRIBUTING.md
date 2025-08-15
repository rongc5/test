# 贡献指南

感谢您对 MyFrame 项目的兴趣！我们欢迎各种形式的贡献。

## 📋 贡献方式

### 🐛 报告问题
- 使用 [GitHub Issues](https://github.com/your-username/MyFrame/issues) 报告 bug
- 提供详细的重现步骤和环境信息
- 包含相关的日志和错误信息

### ✨ 功能请求
- 在 [GitHub Discussions](https://github.com/your-username/MyFrame/discussions) 中讨论新功能
- 描述功能的使用场景和预期行为
- 考虑提供设计草案或原型

### 🔧 代码贡献
1. Fork 本仓库
2. 创建功能分支 (`git checkout -b feature/amazing-feature`)
3. 进行修改并测试
4. 提交代码 (`git commit -m 'Add amazing feature'`)
5. 推送分支 (`git push origin feature/amazing-feature`)
6. 创建 Pull Request

## 🛠️ 开发环境设置

### 系统要求
- Linux/Unix 操作系统
- GCC 4.8+ 或 Clang 3.4+
- CMake 3.5+ (可选)
- OpenSSL 1.1.0+ (启用 SSL 时)

### 编译和测试
```bash
# 克隆仓库
git clone https://github.com/your-username/MyFrame.git
cd MyFrame

# 编译
make -f core.mk

# 编译带 SSL 支持
make -f core.mk ENABLE_SSL=1

# 运行测试
cd test
make all
./test_runner
```

## 📝 代码规范

### C++ 编码标准
- 使用 C++11/14 特性
- 遵循 RAII 原则
- 优先使用智能指针
- 避免原始指针和手动内存管理

### 命名约定
```cpp
// 类名：大驼峰
class HttpProcessor {};

// 函数名：小写下划线
void process_request();

// 变量名：小写下划线
int connection_count;

// 成员变量：下划线前缀
class MyClass {
private:
    int _member_variable;
};

// 常量：大写下划线
const int MAX_CONNECTIONS = 1000;
```

### 文件组织
```
include/        # 公共头文件
core/          # 核心实现
utils/         # 工具类
test/          # 测试代码
examples/      # 示例代码
```

### 头文件规范
```cpp
#ifndef __HEADER_NAME_H__
#define __HEADER_NAME_H__

#include "base_def.h"    // 项目内部头文件
#include <iostream>      // 系统头文件

class MyClass {
public:
    MyClass();
    ~MyClass();
    
    void public_method();
    
private:
    void private_method();
    int _private_member;
};

#endif
```

### 代码格式
- 使用 4 个空格缩进（不使用 Tab）
- 大括号另起一行
- 行长度不超过 120 字符
- 在逻辑块之间添加空行

```cpp
void example_function(int param1, const std::string& param2) 
{
    if (param1 > 0) 
    {
        std::cout << "Processing: " << param2 << std::endl;
        
        for (int i = 0; i < param1; ++i) 
        {
            process_item(i);
        }
    }
}
```

## 🧪 测试要求

### 单元测试
- 新功能必须包含相应的单元测试
- 测试文件命名：`test_[module_name].cpp`
- 确保所有测试通过

### 集成测试
- 修改核心功能时需要运行完整的集成测试
- 包括 HTTP、HTTPS、WebSocket、WSS 功能测试

### 性能测试
- 对性能敏感的修改需要提供性能测试结果
- 包括延迟、吞吐量、内存使用等指标

## 📚 文档要求

### 代码文档
```cpp
/**
 * @brief 处理 HTTP 请求
 * 
 * @param request HTTP 请求对象
 * @param response HTTP 响应对象
 * @return bool 处理是否成功
 */
bool process_http_request(const HttpRequest& request, HttpResponse& response);
```

### README 更新
- 新功能需要更新 README.md
- 包括使用示例和配置说明

### API 文档
- 公共 API 的修改需要更新文档
- 提供使用示例

## 🔄 Pull Request 流程

### PR 标题格式
```
[类型] 简短描述

类型：
- feat: 新功能
- fix: 问题修复
- docs: 文档更新
- style: 代码格式
- refactor: 重构
- test: 测试相关
- chore: 构建/工具相关
```

### PR 描述模板
```markdown
## 修改说明
简要描述这个 PR 的目的和内容

## 修改类型
- [ ] 新功能
- [ ] 问题修复
- [ ] 文档更新
- [ ] 代码重构
- [ ] 性能优化

## 测试情况
- [ ] 单元测试通过
- [ ] 集成测试通过
- [ ] 手动测试通过

## 相关 Issue
修复 #issue_number

## 检查列表
- [ ] 代码遵循项目规范
- [ ] 包含必要的测试
- [ ] 文档已更新
- [ ] 无编译警告
```

### 代码审查
- 所有 PR 需要至少一个维护者的审查
- 确保代码质量和一致性
- 讨论设计决策和实现细节

## 🏷️ 发布流程

### 版本规划
- 遵循语义化版本 (Semantic Versioning)
- 主版本：不兼容的 API 变更
- 次版本：向后兼容的功能增加
- 补丁版本：向后兼容的问题修复

### 发布检查
- [ ] 所有测试通过
- [ ] 文档已更新
- [ ] CHANGELOG.md 已更新
- [ ] 版本号已标记

## 🤝 社区准则

### 行为准则
- 尊重所有贡献者
- 建设性的反馈和讨论
- 包容不同的观点和经验水平

### 沟通方式
- GitHub Issues：问题报告和功能请求
- GitHub Discussions：设计讨论和问答
- Pull Request：代码审查和修改讨论

## 📞 联系方式

如有任何问题，可以通过以下方式联系：
- GitHub Issues
- 项目维护者邮箱

感谢您为 MyFrame 项目做出的贡献！🚀