#include <iostream>

using namespace std;

class A
{
    public:
        template<class T>
        static void show()
        {
            T * ptr = new T;
            ptr->display();
        }

        void display()
        {
            cout << "hello world 111" << '\n';
        }
};

class B:public A
{
    public:
        void display()
        {
            cout << "hello world 222" << '\n';
        }
};

int main(int argc, char *argv[])
{

    B::show<B>();

    return 0;
}
