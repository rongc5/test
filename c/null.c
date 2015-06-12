#include <stdio.h>
#include <string.h>

int main(void)
{
    char *p = NULL /*'\0'*/;

    printf("%d\n", p);
    char t = 0;
    printf("%s\n", t);

    int *ptr = (int *)p;
    if (!ptr)
    {
        printf("hello\n");
    }


    return 0;
}
