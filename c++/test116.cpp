#include <iostream>
#include <string>

using namespace std;

class A
{
    public:
        void operator() ()
        {
            cout << "hello world" << '\n';
        }
};


int main(int argc, char *argv[])
{
    A a;
    a();

    return 0;
}
