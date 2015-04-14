#include <iostream>

using namespace std;

class A {
public:
    int x;
    int y;
};

int main(void)
{
    A a, b;
    a.x = 3;
    cout << sizeof(a)<<":"<<sizeof(A) <<endl;

    return 0;
}
