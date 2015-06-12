#include <iostream>

class women{
    public:
        virtual void sleep() = 0;
        virtual void study() = 0;
};

class poxy:public women{
    public:
        void sleep();
    private:
        

};


int main(int argc, char *argv[])
{
    std::cout<<sizeof(women) << std::endl;

    return 0;
}
