#include <iostream>

using namespace std;

struct B
{
    public:
        B(int c):a(c){}


        int a;
};

struct A
{
        bool operator()(B &pa, B &pb)
        {
            if (pa.a > pb.a)
                cout << "hello world" << '\n';
            else if (pa.a < pb.a)
                cout << "hello bilin" << '\n';
            else
                cout << "hello" << '\n';
        }

};


int main(int argc, char *argv[])
{
    B b(5);
    B c(5);

    //b(c);
    //c(b);
    A a;
    a(b, c);
    
    return 0;
}
