#include <iostream>
#include <stdexcept>
#include <exception>


void Foo()
{
    //throw std::exception("This is my exception");
    throw ("This is my exception");
}

int main(int argc, char *argv[])
{
    try {
        //throw std::exception("This is my exception");
        Foo();
    }catch (std::exception & e){
        std::cout << e.what() << '\n';
    }

    return 0;
}
