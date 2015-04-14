#include <iostream>
#include <string>

using namespace std;

class A{
    public:
        void display();
    private:
        int a;
        string str;
};

void A::display()
{
    std::cout << a << " " << str.c_str() << '\n'; 
}

int main(int argc, char *argv[])
{
    A a;

    a.display();

    return 0;
}
