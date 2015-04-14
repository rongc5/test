#include <iostream>

namespace {
int show();
}

int show()
{
    std::cout<< "hello world" << '\n';
}

int main(int argc, char *argv[])
{
    ::show();
    return 0;
}
