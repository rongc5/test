#include <iostream>
 using namespace std;

class A
{
    public:
        void set();
        void show();
    private:
        enum SWITCH_TYPE
        {
            SWITCH_1,
            SWITCH_2,
            SWITCH_3,
        };
    private:
        SWITCH_TYPE a;
};

void A::set()
{
    a = SWITCH_3;
}

void A::show()
{
    switch (a)
    {
        case SWITCH_1:
            {
                std::cout << "hello 1" << '\n';
            }
            break;
        case SWITCH_2:
            {
                std::cout << "hello 2" << '\n';
            }
            break;
        case SWITCH_3:
            {
                std::cout << "hello 3" << '\n';
            }
            break;

    }
}

int main(int argc, char *argv[])
{
    A a;
    
    a.set();
    a.show();

    return 0;
}
