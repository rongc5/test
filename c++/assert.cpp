#include <cassert>
#include <iostream>

int main(int argc, char *argv[])
{
    std::cout << "argc: "<<argc <<"\n";
    assert(argc > 2);

    std::cout << "hello world\n";

    return 0;
}
