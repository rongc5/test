#include <iostream>
#include <vector>

int main(int argc, char *argv[])
{
    std::vector<int> foo(3, 100), bar;

    for (unsigned i = 0; i < foo.size(); i++){
        std::cout << ' ' << foo[i];
    }
    std::cout<<'\n';

    bar.swap(foo); 

    for (unsigned i = 0; i < bar.size(); i++){
        std::cout << ' ' << bar[i];
    }
    std::cout<<'\n';

    return 0;
}
