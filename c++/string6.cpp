#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>

using namespace std;

int main(int argc, char *argv[])
{
    string str = "hello world";

    char * pstart = (char *)str.c_str();
    pstart = pstart +5;
    char * end = pstart +3;

    string str1(pstart, end);

    printf("%s\n", str1.c_str());
     
    return 0;
}
