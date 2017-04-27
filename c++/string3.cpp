#include <iostream>
#include <string>
#include <stdio.h>

using namespace std;

int main(int argc, char *argv[])
{
    string str;
    str.append("1=");
    str.append("2&");
    str.append("3=");
    str.append("4&");
    
    printf("%s\n", str.c_str());

    return 0;
}

