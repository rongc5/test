#include <iostream>
#include <string>

using namespace std;

class A{
    public:

    A(string & str1){data = str1;}
    protected:
        string data;
};

class B:public A
{
    public:
        B(string &str1):A(str1){}
        void show();
};

void B::show()
{
    cout << data.c_str()<< '\n';
}

int main(int argc, char *argv[])
{
    string str1 = "hello world";
    B b(str1);

    b.show();

    return 0;
}
