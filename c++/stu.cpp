#include <iostream>

using std::cout;
using std::endl;

struct Test1
{
    Test1()
    {
        cout<<"construct Test1"<<endl;
    }

    Test1(const Test1& t1)
    {
        cout << "copy constructor for Test1"<<endl;
        this->a = t1.a; 
    }

    Test1& operator = (const Test1& t1)
    {
        cout << "assignment for Test1" << endl;
        this->a = t1.a;
        return *this;
    }

    int a;
};

struct Test2
{
    Test1 test1;
    Test2(Test1 &t1)
    {
        test1 = t1;
        cout<< "construct Test2" << endl;
    }
};

int main(void)
{
    Test1 t1;
    Test2 t2(t1);

    cout<<sizeof(Test1) << endl;
    return 0;
}
