#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    
    //C++11 提供了原始字符串字面量的写法，可以在一个字符串前方使用 R 来修饰这个字符串，同时，
    //将原始字符串使用括号包裹
    std::string str = R"(C:\File\To\Path)";
    std::cout << str << std::endl;

    return 0;
}
