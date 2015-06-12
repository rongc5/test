#include <iostream>
#include <string>

using namespace std;

class mediator;

class person
{
    public:
        person(mediator * med){_med = med;}
        person(){_med = NULL;}
        virtual void put_msg(string & msg) = 0;
        virtual void get_msg(string & msg) = 0;
        virtual void set_med(mediator * med){_med = med;}

    protected:
        mediator * _med;
};

class mediator
{
    public:
        virtual void set_A(person * per){_A = per;}
        virtual void set_B(person * per){_B = per;}
        virtual void put(string & str, person * per)
        {
            if (per == _A)
            {
                _B->get_msg(str);
            }
            else if (per == _B)
            {
                _A->get_msg(str);
            }
        }

    protected:
        person * _A, * _B;
};


class user: public person
{
    public:

        user(const char * str, mediator * med):_name(str), person(med){}

        virtual void put_msg(string & msg)
        {
            if (_med == NULL)
            {

                cout << "mediator--> set_med()\n";
                return ;
            }

            _med->put(msg, this);
        }

        virtual void get_msg(string & msg)
        {
            cout << "hello " << _name.c_str() <<" ==> :i am " << msg.c_str() << '\n';
        }

    private:
        string _name;
};

class boss: public person
{
    public:

        boss(const char * str, mediator * med):_name(str), person(med){}

        virtual void put_msg(string & msg)
        {
            if (_med == NULL)
            {

                cout << "mediator--> set_med()\n";
                return ;
            }

            _med->put(msg, this);
        }

        virtual void get_msg(string & msg)
        {
            cout << "hello " << _name.c_str() <<" ==> :i am " << msg.c_str() << '\n';
        }

    private:
        string _name;
};

int main(int argc, char *argv[])
{
    mediator * pmed = new mediator;

    person * per_A = new user("user", pmed);
    person * per_B= new boss("boss", pmed);

    pmed->set_A(per_A);
    pmed->set_B(per_B);

    string str("hello");
    per_A->put_msg(str);

    return 0;
}
