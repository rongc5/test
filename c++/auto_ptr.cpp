#include <iostream>
#include <memory>

using namespace std;

class A{
    public:
        A(){
            std::cout << "I am A" << std::endl;
        }
        ~A()
        {
            std::cout << "I am A, and I will disappeared!" << std::endl;
        }

        void show(){
            std::cout<<"hello world\n";
        }
};

int main(int argc, char *argv[])
{
    {
        std::auto_ptr<A> p(new A);
        p->show();
        //p.release();
        p.reset(new A);
    }

    std::cout << "fun ==> " << "main" <<'\n';

    return 0;
}
