#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *p = NULL;

int atexit(void (*funcptr)(void));

void delete(void)
{
    if (p)
        free(p);
    printf("%p is free!\n", p);
}

void test(void)
{
    printf("just test!\n");
}

int main(void)
{
    p = (char *)malloc(32);
    if (!p)
    {
        perror("malloc");
        exit(1);
    }

    atexit(test);
    atexit(delete);
    strcpy(p, "helllo world!");

    printf("%s\n", p);

    exit(0);
}
