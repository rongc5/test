#include <iostream>
#include <string>

using namespace std;

class work;

class state
{
    public:
        virtual void handle(work *)=0;
};

class work
{
    public:
        work(state *st):_st(st){}

        void realdo()
        {
            _st->handle(this);
        }

        ~work()
        {
            delete _st;
        }
    //protected:

        void change_state(state * st)
        {
            delete _st;
            _st = st;
            //_st->handle(this);
        }

    private:
        state * _st;
};

class supper_state: public state
{
    public:
        virtual void handle(work * wk)
        {
            cout << "hello: supper\n";
        }
};

class lunch_state: public state
{
    public:
        virtual void handle(work * wk)
        {
            cout << "hello: lunch:\n";
            wk->change_state(new supper_state);
        }
};

class breakfast_state:public state
{
    public:
        virtual void handle(work * wk)
        {
            cout << "hello: breakfast\n";
            wk->change_state(new lunch_state);
        }
};



int main(int argc, char *argv[])
{
    work * ptr = new work(new breakfast_state);
    ptr->realdo();
    ptr->realdo();
    ptr->realdo();
    
    delete ptr;

    return 0;
}
