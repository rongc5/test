#include <iostream>
#include <memory>

void foo(std::shared_ptr<int> i) {
    (*i)++;
}

int main(int argc, char *argv[])
{
    auto pointer = std::make_shared<int>(10);
    foo(pointer);

    std::cout << *pointer << std::endl;

    return 0;
}
