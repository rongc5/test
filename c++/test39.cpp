#include <set>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    set<int> test;
    set<int>::iterator it;

    for (int i = 0; i < 10; i++)
        test.insert(i);

    for (it = test.begin(); it != test.end(); it++)
        std::cout << *it << std::endl;

    return 0;
}
