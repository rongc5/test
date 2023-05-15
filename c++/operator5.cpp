#include <iostream>
#include <string>
#include <sstream>

class MyClass {
    public:
        //类型 endmsg 用于标记最后一个参数

        //处理非最后一个参数的 operator<<
        template<typename T>
            MyClass& operator<<(const T& obj) {
                ss_ << obj;
                std::cout << obj << ":";
                return *this;
            }

        //处理最后一个参数的 operator<<
        ~MyClass() {
            std::cout << "the last one";
            ss_ << std::endl;
        }

        std::string str() const { return ss_.str(); }

    private:
        std::stringstream ss_;
};

#define LDBUG MyClass()<< "hello" << "345" << 67

int main() {
    MyClass m;
    LDBUG << "98" << "world!" ;
    std::cout << m.str();
    return 0;
}
//
