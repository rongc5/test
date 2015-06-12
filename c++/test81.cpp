#include <iostream>

using namespace std;

class A
{
    public:
        static void show();
    private:
        static int * ptr;
};

int * A::ptr=NULL;

void A::show()
{
    if (ptr)
        cout << "ptr is not null!!!" << '\n';
    else
        cout << "ptr is null!!!" << '\n';
}

int main(int argc, char *argv[])
{
    A a;
    a.show();

    return 0;
}
