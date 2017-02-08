#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int a = 5;

    char buf[12] = {0};

    memcpy(buf, &a, sizeof(a));

    int *ptr = (int *)buf;

    printf("%d\n", *ptr);

    return 0;
}
