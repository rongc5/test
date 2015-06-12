#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{
    umask(0);

    FILE *fp = fopen("tmp", "w");
    if (!fp)
    {
        perror("fopen");
        exit(1);
    }
    fprintf(fp, "hello world!");

    fclose(fp);
    return 0;
}
