#include <iostream>
#include <string>

class MyClass {
    public:
        template <typename... Args>
            MyClass& operator<<(const Args&... args) {
                handle_args(args...);
                return *this;
            }

    private:
        int total_count = 0;

        template <typename T>
            void handle_args(const T& arg) {
                // 如果只有一个参数，即最后一个字符串，则在这里执行相应操作
                std::cout << "last string: " << arg << std::endl;
                total_count++;
                std::cout << "Total count: " << total_count << std::endl;
            }

        template <typename T, typename... Args>
            void handle_args(const T& arg, const Args&... args) {
                // 如果不止一个参数，即不是最后一个字符串，则递归调用 handle_args 处理后面的参数
                std::cout << "string: " << arg << std::endl;
                handle_args(args...);
            }
        
};

int main() {
    MyClass obj;
    obj << "11" << "22" << "33";
    return 0;
}

