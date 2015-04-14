#include <iostream>
#include <string>

using namespace std;

class A
{
    public:
        void show()
        {
            cout << a << " " << _str << '\n';
        }

        A()
        {
            this->show();

            a = 199;
            _str = "qianfeng";

            this->show();
        }

    private:
        int a;
        string _str;

};

int main(int argc, char *argv[])
{
     A a;
    
    return 0;
}
