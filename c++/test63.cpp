#include <iostream>
#include <string>

class A{
public:
     A(const char *ptr = "hello world"):str(ptr){}
    void show(){std::cout << str.c_str() << '\n';}

private:
    std::string str;
};

class B:public A{
    public:
        void show(){std::cout << "qianfeng" << '\n';}
    private:
        int b;
        //A a;
};

int main(int argc, char *argv[])
{
    B b;

    b.show();

    return 0;
}
