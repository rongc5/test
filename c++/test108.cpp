#include <iostream>

using namespace std;

class A
{
    public:
        static void *show(A *ptr)
        {
            cout << "hello world" << '\n';
        }
        void show()
        {
            show(this);
        }
        static int a;
};

int A::a = 99;

int main(int argc, char *argv[])
{
    A a;
    a.show();
    cout << A::a << '\n';

    return 0;
}
