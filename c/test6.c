#include <stdio.h>

int getX(int a, int b)
{
    int c = 0;
    while (a != b){
        if (a > b)
            a = a - b;
        else if (a  < b)
            b  = b - a;
    }

    return a;
}

int main(int argc, char *argv[])
{
    printf("%d\n", getX(12, 16));

    return 0;
}
