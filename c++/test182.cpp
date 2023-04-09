#include <iostream>

template<typename ... T>
auto sum(T ... t) {
    return (t + ...);
}

int main(int argc, char *argv[])
{
    std::cout << sum(1, 2, 3, 4, 5, 6, 7, 8, 9, 10) << std::endl;
    return 0;
}
