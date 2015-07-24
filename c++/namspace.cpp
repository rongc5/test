#include <iostream>

namespace {
    void abc()
    {
        std::cout << "hello world\n";
    }
}

int main(int argc, char *argv[])
{
    ::abc();

    return 0;
}
