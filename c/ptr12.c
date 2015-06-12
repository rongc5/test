#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char *ptr;

    ptr = (char *)malloc(sizeof(10));

    /*free(ptr);*/

    /**ptr = 10;*/
    int a = *ptr;

    /*free(ptr);*/

    /*exit(0);*/

    return 0;
}   
