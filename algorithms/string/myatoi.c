#include <stdio.h>
#include <stdlib.h>

int myatoi(const char *src)
{
    int tmp = 0, flag = 1;
    const char *p = src;

    if (*p == '-'){
        flag = -1;
        p++;
    }
    
    for (p; *p >= '0' && *p <= '9'; p++){
        tmp = tmp*10 + (*p -'0');
    }

    tmp *= flag;
    return tmp;
}

int main(int c, char **v)
{
    if (c != 2)
        return 1;

    printf("%d\n", myatoi(v[1]));

    return 0;
}
