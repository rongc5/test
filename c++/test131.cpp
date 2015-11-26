#include <iostream>

using namespace std;

class A
{
    public:
        int _a;
        A()
        {
            _a = 1;
            _b = 2;
            _c = 3;
        }
    protected:
        int _b;
    private:
        int _c;
        void display()
        {
            cout << "_c: " << _c <<"\n";
        }
};


class B:A
{
    public:
        void show()
        {
            cout << "a: " << _a << "\n";
            cout << "b: " << _b << "\n";
            //cout << "c: " << _c << "\n";   error
        }
};

int main(int argc, char *argv[])
{
    B b;

    b.show();

    A * ptr;

    //ptr = &b;  error

    return 0;
}


