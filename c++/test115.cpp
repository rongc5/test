#include <iostream>
#include <string>

using namespace std;

class A
{
    public:
        virtual void dispaly()
        {
            cout << "hello world" << '\n';
        }
};

class B: public A
{
    public:
        void dispaly()
        {
            string str = "zm";
            cout << str << '\n';

            show();
        }

        void show()
        {
            cout << "qq" << '\n';
        }
};

int main(int argc, char *argv[])
{
    A * ptr = new B;

    ptr->dispaly();


    return 0;
}
