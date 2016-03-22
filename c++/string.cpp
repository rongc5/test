#include <iostream>
#include <string>
#include <sstream>

using namespace std;

class A
{
    public:
        operator std::string () const
        {
            stringstream stream;
            stream << "hello\n";
            //cout << stream.str();
            return stream.str();
        }
};


int main(int argc, char *argv[])
{
    A a;
    //cout << str.c_str();
    //a.string();
    //cout << a;
    string b = a;
    cout << b.c_str();

    return 0;
}
