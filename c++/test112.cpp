#include <iostream>
#include <string>
#include <list>

using namespace std;

class B;
class A
{
    public:
        A(void *p)
        {
            ptr = (B<A> *)ptr;
        }
        static A *gen()
        {
            B<A> * p = new B<A>;
        }
    private:
        B<A> * ptr;
};

class B:public A
{
    public:

       void display()
        {
            cout << "hello world" << '\n';
        }
};


int main(int argc, char *argv[])
{
    A * ptr = new B;
    ptr->set(ptr);
    ptr->display();

    

    return 0;
}
