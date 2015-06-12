#include <iostream>

class women{
    public:
        void sleep();
        void study();
    private:
        int a;
};


void women::sleep()
{
    std::cout<< "sleep" << std::endl;
}

void women::study()
{
    std::cout<< "study" << std::endl; 
}

int main(int argc, char *argv[])
{
    women aa;

    aa.sleep();
    //women::study();
    //std::cout<<sizeof(women) << "hello world" << sizeof(aa) << std::endl;


    return 0;
}
