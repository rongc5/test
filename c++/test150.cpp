#include <map>
#include <iostream>

typedef enum e
{
    ONE = 1,
    TWO = 2,
    THREE = 3
} etype;

int main(int arc, char **argv)
{
    std::map <e, std::string> mmap;
    mmap[THREE] = 3;
    
    return 0;
}
