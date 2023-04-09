#include <iostream>
#include <utility>


auto add = [](auto x, auto y) {
    return x + y;
};

int main(int argc, char *argv[])
{
    std::cout << add(1,2) << std::endl;
    std::cout << add(1.1, 2.2) << std::endl;

    return 0;
}
