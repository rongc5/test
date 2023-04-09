#include <iostream>
#include <string>
#include <vector>


template<typename T, typename U>
class MagicType {
    public:
        T dark;
        U magic;
};


typedef int (*process)(void *);
using NewProcess = int(*)(void *);
template<typename T>
using TrueDarkMagic = MagicType<std::vector<T>, std::string>;


int main(int argc, char *argv[])
{
    TrueDarkMagic<std::string> you;

    return 0;
}
