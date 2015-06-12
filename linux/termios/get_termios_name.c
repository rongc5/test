#include <stdio.h>
#include <string.h>

char array[20];
char *p  = array;

int main(void)
{
    printf("name = %s\n", ctermid(p));

    return 0;
}
