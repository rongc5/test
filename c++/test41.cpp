#include <iostream>

class A{
public:
    static void show();
    //void show();
};

void A::show()
{
    std::cout<<"hello world"<<std::endl;
}

int main(int argc, char *argv[])
{
    A a;
    a.show();

    return 0;
}
