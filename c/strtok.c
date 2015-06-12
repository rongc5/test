#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void)
{
    char buf[120] = "ababcabcdabcdeabcdefgh"; 
    char *src = buf;
    char *dst = "ab";
    char *ptmp = NULL;

    int len = strlen(src);
    int i = 0;

    printf("%s\n", src);
    ptmp = strtok(src, "bde");

    printf("ptmp:%s\n", ptmp);

    for (i = 0; i < len; i++){
        printf("%c", *(src + i));
    }

    printf("\n");

    return 0;
}
