#include <iostream>
#include <string>
#include <stdio.h>
#include <sstream>

using namespace std;

#define format_str(dst, STR)\
    do {\
        stringstream ss; \
        ss << STR; \
        dst = ss.str();\
    }while (0)


int main(int argc, char *argv[])
{
    string str;

    format_str(str, "U_"<<10086<<"hello");

    printf("%s\n", str.c_str());

    return 0;
}
