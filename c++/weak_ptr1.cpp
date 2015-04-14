#include <iostream>
#include <memory>

using namespace std;

class Woman;
class Man;

class Man
{
    private:
        weak_ptr<Woman> _wife;
    public:
        void setWife(shared_ptr<Woman> woman)
        {
            _wife = woman;
        }

        void doSomething()
        {
            if (!_wife.expired())
            {
                cout<<"hello world\n";
                auto sp = _wife.lock();
                //(*sp).doSomething();
            }else {
                cout << "expired!\n";
            }
        }

        ~Man()
        {
            cout<<"kill man\n";
        }
};

class Woman
{
    private:
        shared_ptr<Man> _husband;
    public:
        void setHusband(shared_ptr<Man> man)
        {
            _husband = man;
        }

        void doSomething()
        {
            cout<<"I am working!\n";
        }
        ~Woman()
        {
            cout << "kill woman\n";
        }
};

int main(int argc, char *argv[])
{
    weak_ptr<Man> wm;
    {
        shared_ptr<Man> m(new Man);
        shared_ptr<Woman> w(new Woman);

        if (m && w)
        {
            m->setWife(w);
            w->setHusband(m);
            wm = m;
        }
    }
    shared_ptr<Man> tt = wm.lock();
    if (tt){
        tt->doSomething();
        cout << "tt is vaild\n";
    }

    return 0;
}
