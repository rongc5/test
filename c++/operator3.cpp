#include <iostream>
#include <string>

using namespace std;

class Test
{
    public:
        Test()
        {
            cout<<"Constructors\n";
        }
        void operator()()
        {
            cout << "operator\n";
            return;
        }
        void operator()(const string & str)
        {
            cout << "str:" <<str.c_str()<<'\n';
        }
};

int main(int argc, char *argv[])
{
    Test t;
    
    t();

    Test();

    t("hello world");
    return 0;
}
