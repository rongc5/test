#include <iostream>
//#include <functional>
#include <thread>

using namespace std;

void add(int & a)
{
    a++;
}

int main(int argc, char *argv[])
{
    int t = 0;

    thread thd(add, std::ref(t));
    thd.join();
    cout << "t: " << t <<'\n';

    return 0;
}
