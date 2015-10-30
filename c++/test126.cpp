#include <iostream>
#include <string>

using namespace std;

class A
{
    public:
        A(const char * str):_str(str)
        {}
        A(const A & a)
        {
        }
        A & operator=(const A & a)
        {
            cout << "operator =\n";
            _str = a._str;
            cout << _str.c_str() << "\n";
            return *this;
        }
    private:
        string _str;
};

int main(int argc, char *argv[])
{
    A a("hello"), b("world");

    a = b;

    return 0;
}
