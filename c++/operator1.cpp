#include <iostream>

using namespace std;

class Test
{
    private:
        int x;
        int y;
        friend bool operator<(const Test &a, const Test &b);
    public:
        Test(int x, int y):x(x), y(y){}
        bool operator == (Test & t) const;
};

bool Test::operator == (Test & t) const
{
    if (this->x == t.x)
    {
        return true;
    }

    return false;
}

bool operator<(const Test &a, const Test &b)
{
    if (a.x < b.x)
    {
        return true;
    }

    return false;
}

int main(int argc, char *argv[])
{
    Test t(1, 3), u (2, 5);
    if (t == u)
    {
        cout << "==\n";
    }else if (t < u)
    {
        cout << "<\n";
    }

    return 0;
}


