#include <stdio.h>

int func(int a, int b)
{
    while (a != b)
    {
        if (a > b)
            a = a - b;
        else
            b = b - a;
    }

    return a;
}

int func1(int a, int b)
{
    int c = b;

    while (b)
    {
        c = a%b;
        a = b;
        b = c;
    }

    return a;
}

int main(int argc, char *argv[])
{
    printf("%d %d %d\n", 15, 20, func(15, 20));
    printf("%d %d %d\n", 15, 20, func1(15, 20));
    
    return 0;
}
