#include <iostream>
#include <list>

using namespace std;

class observer;
class work;

class observer
{
    public:
        virtual void display()=0;
        virtual ~observer(){}
};

class work
{
    public:
        void add(observer * obs)
        {
            _mobs.push_back(obs);
        }

        void del(observer * obs)
        {
            _mobs.remove(obs);
        }

        void play()
        {
            for (ITER it = _mobs.begin(); it !=  _mobs.end(); ++it)
            {
                (*it)->display();
            }
        }

    private:
        list<observer *> _mobs;
        typedef list<observer *>::iterator ITER;
};

class A: public observer
{
    public:
        virtual void display()
        {
            cout << "hello:A\n";
        }
        virtual ~A(){}
};

class B: public observer
{
    public:
        virtual void display()
        {
            cout << "hello:B\n";
        }
        virtual ~B(){}
};

class C: public observer
{
    public:
        virtual void display()
        {
            cout << "hello:C\n";
        }
        virtual ~C(){}
};

int main(int argc, char *argv[])
{
    work * wk = new work; 

    observer * ptr1 = new A;
    wk->add(ptr1);
    observer * ptr2 = ptr1;
    wk->add(ptr2);
    observer * ptr3 = new C;
    wk->add(ptr3);
    
    wk->play();



    return 0;
}
