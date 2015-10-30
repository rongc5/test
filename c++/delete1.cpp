#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <memory>

using namespace std;

class base{
    public:
        virtual void display()=0;
        virtual ~base(){}
};

class A:public base{
    public:
        virtual void display()
        {
            cout << "I am A\n";
        }
        virtual ~A()
        {
            cout << "A is destoryed!\n";
        }
    private:
        int a;
};

class B:public base {
    public:
        virtual void display()
        {
            cout << "I am B\n";
        }
        virtual ~B()
        {
            cout << "B is destoryed!\n";
        }
    private:
        string _str;
};

int main(int argc, char *argv[])
{
    //base *bs = new B[3]; error
    //B *bs = new B[5]; ok

    
    /*base *bs = new B[3];

    for (int i = 0; i < 3;i++){
        bs->~base();
        bs++;
    }*///error


    //delete[] bs;
    //
    /* this is ok;
        array<base *, 3> ay = {new A, new B, new A};
        for (int i = 0; i < 3; i++){
            delete ay.at(i);
        }


    */
    
    /*
    {
        vector<shared_ptr<base> > vr;
        vr.push_back(shared_ptr<base>(new A));
        vr.push_back(shared_ptr<base>(new B));
        vr.push_back(shared_ptr<base>(new A));
    }*///ok


    /*{
        array<shared_ptr<base>, 3> ay = {shared_ptr<base>(new A), 
            shared_ptr<base>(new B), shared_ptr<base>(new A)};
            }*///this is ok


    /*
        vector<shared_ptr<base> > *vr = new vector<shared_ptr<base> >;
        vr->push_back(shared_ptr<base>(new A));
        vr->push_back(shared_ptr<base>(new B));
        vr->push_back(shared_ptr<base>(new A));
        delete vr;
        */// this is ok

    return 0;
}
