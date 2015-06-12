#include <iostream>

class A{
    public:
        void display();
    private:
        void show();
        int i;
};

void A::show()
{
    std::cout << i << '\n';
}


void A::display()
{
    show();
}


int main(int argc, char *argv[])
{
    A a;

    a.display();

    return 0;
}
