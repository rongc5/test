#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>


using namespace std;

struct test {
    int id;
    string str;
};

int main(int argc, char *argv[])
{
    test t;

    char buf[100];
    //int ret = sprintf(buf, "%s", "hello world\n");

    //t.str.append(buf);
    memcpy((void *)t.str.c_str(), "hello world", sizeof("hello world"));
    
    printf("%d\n", strlen(t.str.c_str()));
    printf("%c\n", t.str[t.str.length() - 1]);

    return 0;
}
