#include <iostream>
#include <map>

using namespace std;

class test
{
    public:
        int get()
        {
            return a;
        }

        int set(int b)
        {
            a = b;
        }
    private:
        int a;
};


bool operator < (test &t1, test & t2)
{
    //int a = t1.get();
    //int b = t2.get();
    //return a < b;
    return t1.get() < t2.get();
}

int main(int argc, char *argv[])
{
    map<test, int > a;


    return 0;
}
