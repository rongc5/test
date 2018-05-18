#include <iostream>
#include <string>
#include <stdio.h>

using namespace std;

int main(int argc, char *argv[])
{
    {
        char buf[512] = {'\0'};

        string str(buf);
    }

    {
        const char * pstart = "hello world";
        const char * pend = pstart;

        string str(pstart, pend);
        if (str.empty())
        {
            printf("empty \n");
        }
    }

    {
        string str("hello");

        printf("%d\n", str.length());
    }
    
    return 0;
}


