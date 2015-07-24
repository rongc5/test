#include <iostream>
#include <string>

using namespace std;

class A
{
    public:
        void show(){cout << "hello world\n";}
        struct B
        {
            A *a;
        };
    private:
        int x, y;


};

int main(int argc, char *argv[])
{
    A a;
    return 0;
}

