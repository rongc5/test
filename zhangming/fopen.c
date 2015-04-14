#include <stdio.h>

int main(int c, char **V)
{
    FILE *fp = fopen("1.c", "a");

    if (!fp)
    {
        perror("fopen");
        return -1;
    }
    
    char buf[32];
    int ret;

    while (1)
    {
        ret = fread(buf, sizeof(char), 32, fp);
        fwrite(1, sizeof(char), 32, buf);
    }

    fclose(fp);

    return 0;
}
