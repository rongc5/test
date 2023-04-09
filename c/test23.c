#include <stdio.h>

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
    int j = 1;
    for (; i<LOGSIZE; j++, i = 1 << j) {
        printf("%d %d\n", i, j);
    }
    return 0;
}
