#include <iostream>
#include <sstream>

int main(int argc, char *argv[])
{
    std::stringstream ss;
    ss << "hello world";
    ss.clear();
    ss.str("");
    ss << "123456";

    std::cout<< ss.str() << std::endl;

    return 0;
}
