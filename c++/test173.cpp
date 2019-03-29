#include <iostream>
#include <string.h>
#include <algorithm>
#include <string>
#include <stdio.h>

using namespace std;

bool endsWith(const std::string &mainStr, const std::string &toMatch)
{
    if(mainStr.size() >= toMatch.size() &&
            mainStr.compare(mainStr.size() - toMatch.size(), toMatch.size(), toMatch) == 0)
        return true;
    else
        return false;
}

int main(int argc, char *argv[])
{
    printf("%d\n", endsWith("123456.txt", ".txt"));

    return 0;
}
