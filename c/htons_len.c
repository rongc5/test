#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

int main(void)
{   
    char a = 4;
    printf("%d, %d\n", a, htons(a));

    return 0;
}

