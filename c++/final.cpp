#include <iostream>

struct Base {
    virtual void foo() final;
};

struct SubClass1 final: Base {
}; //ok


struct SubClass2 : SubClass1 {
};//error  SubClass1 �� final

struct SubClass3: Base {
    void foo();//error foo �� final
};


