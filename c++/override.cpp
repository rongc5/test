#include <iostream>

struct Base {
    virtual void foo(int);
};

struct SubClass: Base {
    virtual void foo(int) override;
    //virtual void foo(float) override; error �����޴��麯��
};

int main(int argc, char *argv[])
{
    

    return 0;
}
