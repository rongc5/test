#include <iostream>

class A
{
    public:
        A()
        {
            a = 1;
            b = 2; 
            c = 3;
        }

        void set(A * ptr)
        {
            //*this = *ptr;
            this->a = ptr->a;
            this->b = ptr->b;
            this->c = ptr->c;
        }
    
        void set_A(int a)
        {
            this->a = a;
        }

        void set_B(int b)
        {
            this->b = b;
        }

        void set_C(int c)
        {
            this->c = c;
        }

        void display()
        {
            std::cout << "a: " << a << " b: " << b << " c: " << c << '\n';
        }
        

    private:
        int a, b, c;
};

int main(int argc, char *argv[])
{
    A a;
    
    a.display();
    a.set_A(9);
    a.set_B(10);

    A * pa = new A();
    pa->set(&a);

    pa->display();

    return 0;
}
