#include <iostream>
#include <memory>

using namespace std;

class A
{
    public:
        A()
        {
            cout<<"A\n";
        }
        virtual ~A()
        {
            cout<<"~A\n";
        }
};

class B:public A
{
    public:
        B()
        {
            cout<<"B\n";
        }
        virtual ~B()
        {
            cout<<"~B\n";
        }
};

int main(int argc, char *argv[])
{
    shared_ptr<B> b;
    {
        cout<<"b: "<<b.use_count() <<"\n";
        shared_ptr<A> X(new B);

        cout<<"X: "<<X.use_count() <<"\n";

        //b = static_pointer_cast<B>(X);
        b = dynamic_pointer_cast<B>(X);
        cout<<"X: "<<X.use_count() <<"\n";
    }
    cout<<"b: "<<b.use_count() <<"\n";

    return 0;
}
