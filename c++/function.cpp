#include <iostream>
#include <functional>

int foo(int para) {
    return para;
}

int main(int argc, char *argv[])
{
    std::function<int(int)> func = foo;

    int important = 10;

    std::function<int(int)> func2 = [&](int value) -> int {
        return 1+value+important;
    };

    std::cout << func(10) << std::endl;
    std::cout << func2(10) << std::endl;

    return 0;
}
