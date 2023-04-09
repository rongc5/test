#include <iostream>

using namespace std;


/*
 *C++11有一些这样的改善，这种改善保证写出的代码比以往任何时候的执行效率都要好。这种改善之一就是生成常量表达式，允许程序利用编译时的计算能力。
 需要计算一个编译时已知的常量，比如特定值的sine或cosin？确实你亦可以使用库函数sin或cos，但那样你必须花费运行时的开销。使用constexpr，你可以创建一个编译时的函数，它将为你计算出你需要的数值。用户的电脑将不需要做这些工作。
 */

#define LEN 10

int len_foo() {
    int i = 2;
    return i;
}

constexpr int len_foo_constexpr() {
    return 5; 
}

constexpr int fibonacci(const int n) {
    return n == 1 || n == 2 ? 1 : fibonacci(n-1)+fibonacci(n-2);
}

int main(int argc, char *argv[])
{
    int len = 10;

    const int len_2 = len + 1;
    constexpr int len_2_constexpr = 1 + 2 + 3;
    char arr_4[len_2_constexpr];

    char arr_6[len_foo_constexpr() + 1];

    std::cout << fibonacci(10) << std::endl;
    std::cout << fibonacci(10) << std::endl;
    std::cout << len_2_constexpr << std::endl;


    return 0;
}
