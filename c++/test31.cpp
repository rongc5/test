#include <iostream>

class A{
    public:
        void show();
    private:
        class B{
            public:
            void display();
        };
        B data;
};

void A::B::display()
{
    std::cout<<"hello world"<<std::endl;
}

void A::show()
{
    data.display();
}

int main(int argc, char *argv[])
{
    A a;

    a.show();

    return 0;
}
