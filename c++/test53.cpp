#include <iostream>
#include <string>

using namespace std;

class A{
public:
    A(int a):a(a){}
    void set(int a)
    {
        this->a = a;
    }
    void display()
    {
        cout << a << '\n';
    }
private:
    int a;
};

int main(int argc, char *argv[])
{
    A a(3);

    a.display();
    cout << sizeof(a) << " " << sizeof(A) << '\n';

    a.set(9);
    a.display();

    return 0;
}

