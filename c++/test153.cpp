#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>

using namespace std;


int main(int argc, char *argv[])
{
    char buf[12] = "hello";
    string str;
    
    str.append(buf, 3);
    str.erase(0, 0);

    printf("%d %d\n", str.size(), str.length());
    
    return 0;
}
