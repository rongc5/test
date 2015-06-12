#include <iostream>
#include <typeinfo>

using namespace std;

template<class T>
class A{
    public:
        T g(T a, T b);
        A();
};

template < class T1, class T2 >
class B
{
    public:
        void g();
        B();
};

template< class T1, class T2 >
B<T1, T2>::B()
{
    cout << "type is" << typeid(T1).name()<<typeid(T2).name()<<endl;
}

template <class T>
void g(const T * a)
{
    T b;
    cout <<"hello" << typeid(b).name()<<endl;
}

int main(void)
{
    B< string, string > b;
    
    int c = 3;

    g(&c);
    return 0;
}
