#include <iostream>
#include <memory>

void foo(const std::shared_ptr<int> & i)
{
    (*i)++;
}

int main(int argc, char *argv[])
{
    auto sp = std::make_shared<int>(12);

    foo(sp);

    std::cout<< *sp <<'\n';

    return 0;
}
