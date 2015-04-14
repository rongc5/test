#include <iostream>

using namespace std;

class A
{
    public:

    void display()
    {
        if (_ptr == NULL)
        {
            cout << "null\n";
        }
    }

    private:
        int * _ptr;
};


int main(int argc, char *argv[])
{
    A a;

    a.display();

    return 0;
}
