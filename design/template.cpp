#include <iostream>
#include <string>

using namespace std;

class base
{
    public:
        virtual ~base(){}
        virtual void doplay() = 0;
        virtual void play()
        {
            cout << "base"<<'\n';
            doplay();
        }
};

class A: public base
{
    public:
        A(const char *str):name(str){}
        virtual void doplay(){ cout << "hello world" << '\n';}

    private:
        string name;
};

int main(int argc, char *argv[])
{
    base * ptr = new A("qq");

    ptr->play();

    return 0;
}
