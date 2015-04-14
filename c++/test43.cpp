#include <sstream>
#include <iostream>

int main(int argc, char *argv[])
{
    std::stringstream stream;
    int first, second;
    stream<<"456";
    std::cout<<stream.str() <<'\n';

    stream>>first;
    std::cout<<stream.str() <<'\n';
    std::cout<<first <<'\n';
    stream.clear();

    stream<<true;
    stream>>second;
    std::cout<<second <<'\n';

    return 0;
}
