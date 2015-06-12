#include <iostream>

using namespace std;

class base
{
    public:
        virtual void doplay()=0;
        virtual ~base(){}
};

class base1:public base
{
    public:
        virtual void doplay()
        {
            cout << "hello world" << '\n';
        }
        virtual ~base1(){}
};

class base2:public base
{
    public:
        virtual void doplay()
        {
            cout << "richard zhang\n"; 
        }
        virtual ~base2(){}
};

class strategy
{
    public:
        void set(base *ptr)
        {
            _ptr = ptr;
        }

        void doaction()
        {
            _ptr->doplay();
        }

    private:
        base * _ptr;
};

int main(int argc, char *argv[])
{
    base * ptr1 = new base1;
    base * ptr2 = new base2;

    strategy * ptr = new strategy;

    ptr->set(ptr1);
    ptr->doaction();

    ptr->set(ptr2);
    ptr->doaction();

    return 0;
}
