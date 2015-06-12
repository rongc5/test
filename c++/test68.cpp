#include <iostream>
#include <string>

template<class T>
class singleton_base
{
    public:
        static T *get_instance()
        {       
            return _p_instance;
        }
        static void set_instance(T *instance)
        {
            _p_instance = instance;
        }
    private:
        static T *_p_instance;  
};

template<class T>
T* singleton_base<T>::_p_instance = NULL;

class A{
    public:
        void show();
        void set(int c, const std::string & str1);
    private:
        int a;
        std::string str;
};

void A::show()
{
    std::cout << a << " " << str.c_str() << '\n';
}

void A::set(int c, const std::string & str1)
{
    a = c;
    str = str1;
}

void fun1()
{
    A * ptr1 = new A;
    
    singleton_base<A>::set_instance(ptr1);
}

void fun2()
{
    //A *ptr = singleton_base<A>::get_instance();
    A *ptr = new A;

    ptr->set(9, "hello world");
}


void fun3()
{
    A *ptr = singleton_base<A>::get_instance();

    ptr->show();
}

int main(int argc, char *argv[])
{
    fun1();

    fun2();

    fun3();

    return 0;
}
