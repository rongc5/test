#include <iostream>

using namespace std;

class base
{
    public:
        virtual void done()=0;
        virtual ~base(){}
};

class A: public base
{
    public:
        virtual void done()
        {
            cout << "hello world" << '\n';
        }
        virtual ~A(){}
};

class proxy: public base
{
    public:
        proxy():_m(NULL){}
        virtual ~proxy(){}
        virtual void done()
        {
            if (_m == NULL)
            {
                _m = new A;
            }

            _m->done();
        }
    private:
        base * _m;
};


int main(int argc, char *argv[])
{
    base * ptr = new proxy;

    ptr->done();

    delete ptr;

    return 0;
}
