#include <iostream>
#include <string>
#include <vector>

using namespace std;

class A
{
    public:
        static void display();
    private:
        static int a;
};

int A::a = 99;

void A::display()
{
    cout << a << '\n';
}

int main(int argc, char *argv[])
{
    
    A::display();
    //B::display();


    return 0;
}
