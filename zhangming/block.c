#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    char buf[256];
    int ret;

    while (1)
    {
        ret = read(0, buf, 256);
        buf[ret] = '\0';
        printf("read:%s", buf);
    }
}

