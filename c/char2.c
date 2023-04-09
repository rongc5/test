#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    char c='y';

    char a[26] = {'0'};
    
    char m = 'a';
    for (int i =0; i <= 26; i++)
        a[i] = m + i;
    //for (c = 0; c < 127; c++)
        printf("%c\n", a[((c+2)-'a')%26]);
        printf("%d\n", c+2);

    return 0;
}
