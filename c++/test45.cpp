#include <iostream>
#include <vector>

int main(int argc, char *argv[])
{
    std::vector<int> test(1000, 10);

    std::cout<< test.size() << '\n';
    std::vector<int>().swap(test);
    
    std::cout<< test.size() << '\n';


    return 0;
}
