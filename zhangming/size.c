#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

int main(int c, char **v)
{
    if (c != 2)
    {
        fprintf(stderr, "usage...\n");
        exit(1);
    }

    struct stat buf;
    int ret;

    ret = stat(v[1], &buf);
    if (-1 == ret)
       {
        perror("stat");
        exit(1);
       }

    printf("File size : %lld", (long long)buf.st_size);

    return 0;
}
