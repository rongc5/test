#include <iostream>
#include <memory>

using namespace std;

class A:public enable_shared_from_this<A>
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
        void aa()
        {
            std::shared_ptr<A> p=shared_from_this();
            cout << "aa: " << p.use_count() <<"\n";
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

        void bb()
        {
            std::shared_ptr<B> p=std::dynamic_pointer_cast<B>(shared_from_this());
            cout << "bb: " << p.use_count() <<"\n";
        }
};

int main(int argc, char *argv[])
{
    shared_ptr<B> b;
    {
        cout<<"b: "<<b.use_count() <<"\n";
        shared_ptr<A> X(new B);

        cout<<"X: "<<X.use_count() <<"\n";
        X->aa();

        //b = static_pointer_cast<B>(X);
        b = dynamic_pointer_cast<B>(X);
        cout<<"X: "<<X.use_count() <<"\n";
        b->bb();

    }
    cout<<"b: "<<b.use_count() <<"\n";

    return 0;
}
