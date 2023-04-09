#include <iostream>

struct Base {
    virtual void foo(int);
};

struct SubClass: Base {
    virtual void foo(int) override;
    //virtual void foo(float) override; error 基类无此虚函数
};

int main(int argc, char *argv[])
{
    

    return 0;
}
