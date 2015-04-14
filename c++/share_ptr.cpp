#include <iostream>
#include <memory>

class A{
    public :
        A(){
            std::cout << "A will appear\n";
        }
        ~A(){
            std::cout << "A will disappear\n";
        }
        void work(){
            std::cout << "A will work\n";
        }
};

int main(int argc, char *argv[])
{   
    {
        std::shared_ptr<A> s1(new A);
        //std::shared_ptr<>
        s1->work();
    }

    {
        std::shared_ptr<A> sh1(new A);
        std::shared_ptr<A> sh2(sh1);

        std::cout << "use count " << sh1.use_count() << "\n";
        std::cout << "use count " << sh2.use_count() << "\n";
    }

    std::cout<< "hello over\n";
    return 0;
}

