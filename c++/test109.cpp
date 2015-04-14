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
            cout << "hello world" << '\n';
        }
};

int main(int argc, char *argv[])
{

    A::show<A>();

    return 0;
}
