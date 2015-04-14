#include <iostream>
#include <string>

using namespace std;

class base
{
    public:
        virtual void play() = 0;
        virtual ~base(){}
};

class rebase
{
    public:
        int QQ()
        {
            cout << "hello world\n";

            return 0;
        }
};

class adapter1: public base, private rebase
{
    public:
        virtual ~adapter1(){}
        virtual void play()
        {
            rebase::QQ();
        }
};

class adapter2: public base
{
    public:
        adapter2(rebase * reb):re(reb){}
        virtual void play()
        {
            re->QQ();
        }

    private:
        rebase * re;
};

int main(int argc, char *argv[])
{
    base * ptr1 = new adapter1;
    ptr1->play();

    base *ptr2 = new adapter2(new rebase);
    ptr2->play();

    return 0;
}
