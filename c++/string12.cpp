#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <vector>
#include <string.h>

using namespace std;


int main(int argc, char *argv[])
{
    char buf[64] = "hello world";
    string str;

    str.append(buf, 64);

    string s1(str);

    cout<< str<< ":"<< s1.size() << ":" << strlen(str.c_str()) << endl;

    return 0;
}
