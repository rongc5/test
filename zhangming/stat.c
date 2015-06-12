#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

int main(int c, char **v)
{
    if (c != 2) {
        fprintf(stderr, "Usage...\n");
        exit(1);
    }

    struct stat buf;
    int ret;
    char * ptr;

    ret = lstat(v[1], &buf);
    if (ret == -1) {
        perror("stat");
        exit(1);
    }

    if (S_ISREG(buf.st_mode))
        ptr = "Regular file";
    else if (S_ISDIR(buf.st_mode))
        ptr = "Directory file";
    else if (S_ISCHR(buf.st_mode))
        ptr = "Character file";
    else if (S_ISBLK(buf.st_mode))
        ptr = "Block file";
    else if (S_ISLNK(buf.st_mode))
        ptr = "Symbolic file";
    else if (S_ISFIFO(buf.st_mode))
        ptr = "Fifo file";
    else if (S_ISSOCK(buf.st_mode))
        ptr = "Socket file";

    printf("File %s is a %s\n", v[1], ptr);
    printf("ACCESS:%s\n", (char *)ctime(&buf.st_atime));
    return 0;
}
