#include <iostream>
#include <memory>

using namespace std;


int main(int argc, char *argv[])
{
    shared_ptr<int> b(new int);
    *b = 5;
    cout<<"b: "<< *b <<"\n";

    return 0;
}
