#include <iostream>
#include <string>


class A{
    public:
        A(int c, const std::string str = "hello"):a(c), b(str){}
        A(){}
        //A & operator = (const A &dst)
        //{
            //this->a = dst.a;
            //this->b = dst.b;
        //}
        void show();
        bool operator == (const A & dst)
        {
            return this->a == dst.a && this->b == dst.b;
        }
    private:
        int a;
        std::string b;
};

void A::show()
{
    std::cout<< a << " " << b.c_str() << '\n';
}

int main(int argc, char *argv[])
{
    A a(5), b;

    a.show();
    if (a == b)
        std::cout << "equal" << '\n';
    else
        std::cout << "not equal" << '\n';

    b = a;

    b.show();

    if (a == b)
        std::cout << "equal" << '\n';
    else
        std::cout << "not equal" << '\n';
    
    return 0;
}
