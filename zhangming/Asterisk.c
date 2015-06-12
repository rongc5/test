#include <stdio.h>
//#include <conio.h>

int main(void)
{
    char tmp;
    int i = 0;

    printf("input password:\n");
    for (i; i < 6; i++)
    {
        tmp = getchar();
        printf("*");
    }
    printf("\n");
    return 0;
}

