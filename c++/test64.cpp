#include <iostream>
#include <string>
#include <list>

int main(int argc, char *argv[])
{
    std::list<std::string> *ptr;

    ptr = new std::list<std::string>;

    ptr->push_back("hello world");

    std::list<std::string>::iterator it;

    it = ptr->begin();

    std::cout << *it << '\n';

    return 0;
}
