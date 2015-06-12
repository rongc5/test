#include <stdio.h>
#include <stdlib.h>

int init_val = 10;
int uinit_val;

int main(void)
{
    int a;
    static int b = 9;
    int * p;

    p = (int *)malloc(sizeof(int));
    if (!p)
    {
        perror("malloc");
        exit(1);
    }

    printf("CODE:%p\n", main);
    printf("DATA:%p\n", &init_val);
    printf("DATA:%p\n", &b);
    printf("BSS:%p\n", &uinit_val);
    printf("HEAP:%p\n", p);
    printf("STACK:%p\n", &a);

    free(p);
    return 0;
}
