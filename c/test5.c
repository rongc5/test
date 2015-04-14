#include<stdio.h>

unsigned int myabs(int);

int main(void)
{
    int n=0x80000000;
    printf("%u\n", myabs(n));

    return 0;
}
unsigned int myabs(int n)
{
    printf("%d\n",n);
    if(n<0)
    {
        return -n;
    }
    else
    {
        return n;
    }
}
