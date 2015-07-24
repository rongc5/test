#include <iostream>

using namespace std;

class Test
{
    public:
        Test()
        {
            cout<<"Constructors\n";
        }
        void operator()()
        {
            cout << "operator\n";
            return;
        }
};

int main(int argc, char *argv[])
{
    Test t;
    
    t();

    return 0;
}
