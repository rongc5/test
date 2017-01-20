#include <iostream>
#include <unistd.h>

using namespace std;

class A{
    public:
        A(){}
        ~A(){
            cout << "~A()\n";
        }

        void destory()
        {
            delete this;
        }
};


int main(int argc, char *argv[])
{
    //A a;

    A * a = new A;

    a->destory();
    while (1)
        sleep(1);
    return 0;
}
