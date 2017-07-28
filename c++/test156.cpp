#include <string>
#include <iostream>
#include <stdio.h>

using namespace std;

int main(int argc, char *argv[])
{
    string str;
    str.append("hello:");
    str.append(" world\r\n");
    str.append("zhang:");
    str.append(" m\r\n");

    printf("%s", str.c_str());

    return 0;
}
