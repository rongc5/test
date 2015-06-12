#include <iostream>

using namespace std;

class A
{
    public:
        static void *show(A *ptr)
        {
            ptr->show();
        }
        void show()
        {
            cout << "hello world" << '\n';
        }
        static int a;
};

int A::a = 99;

int main(int argc, char *argv[])
{
    A a;
    a.show(&a);
    cout << A::a << '\n';

    return 0;
}
