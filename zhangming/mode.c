#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int c, char **v)
{
    if (c != 2)
    {
        fprintf(stderr, "usage...\n");
        exit(1);
    }
    
    struct stat buf;
    int ret;
//    char ptr;

    ret = stat(v[1], &buf);
    if (ret == -1)
    {
        perror("stat");
        exit(1);
    }

    if (buf.st_mode & S_IRUSR)
        printf("r");
    else
        printf("-");
    if (buf.st_mode & S_IWUSR)
        printf("w");
    else
        printf("-");
    if (buf.st_mode & S_IXUSR)
        printf("x");
    else 
        printf("-");


    if (buf.st_mode & S_IRGRP)
        printf("r");
    else
        printf("-");
    if (buf.st_mode & S_IWGRP)
        printf("w");
    else
        printf("-");
    if (buf.st_mode & S_IXGRP)
        printf("x");
    else 
        printf("-");

    
    if (buf.st_mode & S_IROTH)
        printf("r");
    else
        printf("-");
    if (buf.st_mode & S_IWOTH)
        printf("w");
    else
        printf("-");
    if (buf.st_mode & S_IXOTH)
        printf("x");
    else 
        printf("-");

    return 0;
}
