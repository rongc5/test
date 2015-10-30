#include <iostream>

using namespace std;

class base
{
    public:
        base(int idx):_idx(idx){}
        base & operator+=(const base &be);
        void display();
    private:
        int _idx;
};

base & base::operator+=(const base &be)
{
    _idx += be._idx;

    return *this;
}

void base::display()
{
    cout << _idx << '\n';
}
int main(int argc, char *argv[])
{
    base a(1), b(9);

    a += b;
    
    a.display();

    return 0;
}
