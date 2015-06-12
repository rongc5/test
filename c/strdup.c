#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void)
{
    const char *p = "hello, world";
    char *ptr;
    ptr = strdup(p);

    printf("%s\n", ptr);
    printf("%p, %p\n", ptr, p);

    free(ptr);
    return 0;
}
