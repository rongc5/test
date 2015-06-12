#include <stdio.h>
#include <stdlib.h>

int main(int c, char **v)
{
    if (c != 3){
        perror("usage...");
        return -1; 
    }

    char *s1 = v[1];
    char *s2 = v[2];

    printf("%d\n", v[1]-v[2]);

    return 0;
}
