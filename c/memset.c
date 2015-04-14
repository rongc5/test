#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(void)
{
    char buf[20];
    memset(buf, 0, 20);

    memcpy(buf, "hello", 4);

    printf("buf:%s\n", buf);

    return 0;
}
