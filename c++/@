#include <iostream>
#include <memory>

using namespace std;

class FOO{

    public :
        FOO()
        {
            cout << "FOO()" <<'\n';
        }

        ~FOO()
        {
            cout << "~FOO()\n";
        }
};

void g(shared_ptr<FOO>  foo)
{
    cout << "I also have a Foo with refcount " << foo.use_count() << endl;
}

//void f(shared_ptr<FOO> foo)
//{
    //cout << "I have a Foo with refcount " << foo.use_count() << endl;
    //g(foo);
//}

int main()
{
    {
        shared_ptr<FOO> foo(new FOO());
    //cout << "Here the refcount is " << foo.use_count() << endl;
    g(foo);
    //cout << "Here the refcount is again " << foo.use_count() << endl;
    //{
        //shared_ptr<FOO> ptr = foo;
        
        //cout << "2 Here the refcount is " << foo.use_count() << endl;
    //}


    cout << "3 Here the refcount is " << foo.use_count() << endl;
    }
    cout << "So here it will go out of scope and reach 0" << endl;
    return 0;
}
