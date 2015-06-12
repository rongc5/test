#include <iostream>

using namespace std;

class A{
    public:
    A(){cout<<"generate !!"<<endl;}
    ~A(){cout<<"destory!!"<<endl;}
private:
        int a;
};

int main(int argc, char *argv[])
{
    A *p = new A();
    delete p;

    return 0;
}
