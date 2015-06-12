#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int alloc()
{
    char *ptr;

    for (int i = 0;  i < 10; i++)
        ptr = (char *)malloc(sizeof(10));

    free(ptr);

    return 0;
}   
