#include <iostream>

template <auto value> void foo() {
    std::cout << value << std::endl;
}

int main(int argc, char *argv[])
{
    foo<100>();
    return 0;
}
