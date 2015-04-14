#include <iostream>
#include <string>

using namespace std;

class cBase {
    string name;
    public:
    cBase(string &s):name(s)
    {
        cout << "BASE:" << name << endl;
    }
    ~cBase(){ cout << "~BASE" << endl;}
};

class cDerive:public cBase{
    int age;
    public:
    cDerive(string &s, int a):cBase(s), age(a)
    {
        cout << "DERIVE:" << age << endl;
    }
    ~cDerive(){cout << "~DERIVE" << endl;}
};

int main()
{
    string ss = "john";
    cDerive d(ss, 27);

    return 0;
}
