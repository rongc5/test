#include <iostream>

using namespace std;

class point {
    float x, y;

    public:
    void set_xy(float a, float b);
    void print(void) 
    {
        cout<<x<<":"<<y<<endl;
    }
};

void point::set_xy(float a, float b)
{
    x = a;
    y = b;
}

int main(void)
{
    point a;
    a.set_xy(4, 6);
    a.print();
    cout << sizeof(a) << endl;
    return 0;
}
