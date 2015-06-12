#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
void fun(void)
{
    printf("I am exitting!\n");

    return;
}

int main(void)
{
    atexit(fun);
    

    return 0;
}
