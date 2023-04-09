#include <iostream>

using foo = void(int);

void functional(foo f) {
    f(1);
}

int main(int argc, char *argv[])
{
    auto f = [](int value) {
        std::cout << value << std::endl;
    };

    functional(f);

    f(2);


    return 0;
}
