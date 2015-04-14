#include <stdio.h>
#include <string.h>

int get(char src[], int slen, int dst[], int dlen)
{
    memset(dst, 0, dlen);

    char * ptr = src;
    int tmp = 0;
    for ( ; *ptr != '\0'; ptr++)
    {
        tmp = *ptr;
        dst[tmp]++;
    }

    return 0;
}

//返回数组中最大值的下镖
int max(int dst[], int len)
{
    int max = dst[0];
    int flag = 0, i = 0;
    for (i = 0; i < len; i++)
    {
        if (max < dst[i])
        {
            max = dst[i];
            flag = i;
        }
    }

    return flag;
}


int main(int argc, char *argv[])
{
    char a[] = "qianfeng, hello world";

    int b[256]= {0};

    get(a, sizeof(a)/sizeof(a[0]), b, sizeof(b)/sizeof(b[0]));
    int flag = max(b, sizeof(b)/sizeof(b[0]));
    printf("the max char %c, and the length is: %d\n", flag, b[flag]);

    return 0;
}
