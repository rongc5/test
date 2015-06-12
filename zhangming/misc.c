#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    FILE *fp;
    char buf[10];
    int ret;

    fp = fopen("1.c", "r+");
    if (!fp)
    {
        perror("fopen");
        exit(1);
    }

    ret = fread(buf, sizeof(char), 10, fp);
    printf("fread: %s\n", buf);
    printf("current file position is %ld\n", ftell(fp));
//    fwrite("hello", 1, 5, fp);
    fseek(fp, 10, SEEK_END);
    fwrite("hello", 1, 5, fp);

    fclose(fp);

    return 0;

}
