#include <iostream>

using namespace std;

template <class _T>
class A
{
    public:
        typedef _T a_type;
};

class B
{
    public:
        typedef A<int> A_TYPE;
        typedef A_TYPE::a_type b_type;
};



int main(int argc, char *argv[])
{
    return 0;
}
