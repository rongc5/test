#include <iostream>

using namespace std;

class B
{
    public:
        B(int c):a(c){}
        bool operator()(B &pb)
        {
            if (this->a > pb.a)
                cout << "hello world" << '\n';
            else if (this->a < pb.a)
                cout << "hello bilin" << '\n';
            else
                cout << "hello" << '\n';
        }

        int a;
};


int main(int argc, char *argv[])
{
    B b(5);
    B c(5);

    //b(c);
    c(b);
    
    return 0;
}
