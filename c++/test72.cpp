#include <iostream>
using namespace std;

class A
{
public:
template<class T>
void f(T t) { cout << t << '\n'; }

template<class T>
T g() { T t;cout << t << '\n'; return t;}

};

int main()
{
A a;
a.f<int>(1);
a.g<string>();
return 0;
}
