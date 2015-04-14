#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void get(char **p)
{
    *p = NULL;
}


int main(void)
{

    char *p;
    
    get(&p);

    if (!p)
    printf("hello world\n");


    return 0;
}
