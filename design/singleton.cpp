#include <iostream>

using namespace std;

class A
{
    public:
        virtual void done()
        {
            cout << "hello world" << '\n';
        }

        virtual ~A(){}
};

class B: public A
{
    public:
        virtual void done()
        {
            cout << "zhang ming" << '\n';
        }

        virtual ~B(){}
};

template<class T>
class single
{
    public:
        void setinstance(T *ptr)
        {
            _ptr = ptr;
        }

        T * getinstance()
        {
            if (_ptr == NULL)
            {
                _ptr = new T;
            }

            return _ptr;
        }

    private:
        static T * _ptr;
};

template<class T>
T * single<T>::_ptr = NULL;

int main(int argc, char *argv[])
{
   //single<A> * ptr = new single<A>;
   single<A> * ptr = new single<A>;
   A * pta = new B;
   ptr->setinstance(pta);
   ptr->getinstance()->done();

    return 0;
}
