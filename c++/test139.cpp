#include <iostream>
#include <string>

using namespace std;

class test{

    public:
        const string & show()
        {
            _str = "hello world";
            return _str;
        }
    private:
        string _str;
};


int main(int argc, char *argv[])
{
    test t;

    //string str = t.show()[];

    //t.show()[0] = 's';

    return 0;
}
