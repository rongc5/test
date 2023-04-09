#include <iostream>
#include <array>
#include <algorithm>

int main(int argc, char *argv[])
{
    std::array<int, 2> arr = {1,2};

    std::cout << arr.empty() << std::endl;

    std::cout << arr.size() << std::endl;
    
    for (auto & i: arr)
    {
        std::cout << i << std::endl;
    }


    std::sort(arr.begin(), arr.end(), [](int a, int b){
                return a > b;
            });

    for (auto & i: arr)
    {
        std::cout << i << std::endl;
    }


    return 0;
}
