#include <iostream>

using namespace std;

class A
{
    public:
        static void dosomething();
};

void A::dosomething()
{
    cout << "dosomething\n";
}

template<typename  T>
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
            T::dosomething();
            return;
        }
};

int main(int argc, char *argv[])
{
    //Test t;
    
    //t();

    Test<A>()();
    return 0;
}
