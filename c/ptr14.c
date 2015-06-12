#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char * fn1()
{
    return (char *)malloc(100);
}

char * fn2()
{
    return (char *)malloc(200);
}

char * fn3()
{
    return (char *)malloc(300);
}

int main(int argc, char *argv[])
{
    fn1();
    fn2();
    fn3();
    
    return 0;
}
