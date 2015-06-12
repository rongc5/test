#include <stdio.h>
#include <string.h>

int main(void)
{
    printf("%%m, %%d\n");
    char buf[15];
    sprintf(buf, "'%%Y-%%m-%%d'");

    printf("%s\n", buf);

    return 0;
}
