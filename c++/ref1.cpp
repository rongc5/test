#include <iostream>
#include <thread>

int add(int x, int y) {
    return x+y;
}


int main(int argc, char *argv[])
{
    [out = std::ref(std::cout << "Result from C code: " << add(1, 2))](){
        out.get() << ".\n";
    }();

    return 0;
}
