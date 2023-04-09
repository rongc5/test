#include <atomic>
#include <iostream>

struct A {
    float x;
    int y;
    long long z;
};

int main(int argc, char *argv[])
{
    std::atomic<A> a;

    std::cout << std::boolalpha << a.is_lock_free() << std::endl;

    return 0;
}
