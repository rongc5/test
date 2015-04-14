#include <utility>
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    std::pair<int, std::string> point;

    point.first = 10;
    point.second = "hello world";

    std::cout<< point.first <<" "<< point.second << std::endl;

    return 0;
}
