#include <iostream>
#include <stdio.h>
#include <string.h>

enum LogType {
    LOGFATAL = 1,
    LOGWARNING = 2,
    LOGNOTICE = 4,
    LOGTRACE = 8,
    LOGDEBUG = 16, 
    LOGSIZE
};

int main(int argc, char *argv[])
{
    int i = LOGFATAL;
    for (;i < LOGSIZE; i++) {
        printf("%d\n", i);
    }

    return 0;
}
