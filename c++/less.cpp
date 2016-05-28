#include <iostream>
#include <functional>

using namespace std;

template <typename A, typename B, typename U = less<int> >
bool m(A a, B b, U u = U())
{
    return u(a, b);
}

int main(int argc, char *argv[])
{
    cout << m(11, 15) << '\n';
    
    return 0;
}
