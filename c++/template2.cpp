#include <iostream>
#include <string>

using namespace std;

template <class _T>
class compare
{
    public:
        void show()
        {
            _T t;
            t.sdssgsg();
        }
};

template <>
void compare<int>::show()
{
    cout << "I am int\n";
}


template <>
void compare<string>::show()
{
    cout << "I am str\n";
}

int main(int argc, char *argv[])
{
    compare<int> t;
    t.show();

    compare<string> h;
    h.show();

    return 0;
}
