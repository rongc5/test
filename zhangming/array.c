#include <stdio.h>

char * pter(void)
{
    char buf[] = {'h', 'e', 'l', 'l', 'o', '\0'};
    
    printf("%s\n", buf);
    return buf;
}

int main(void)
{
    int a[5] = {0, 1, 2, 3, 4};
    int i = 3;
    printf("%d\n", *a);

    char * p = pter();
    printf("%c\n", p);
    return 0;
}
