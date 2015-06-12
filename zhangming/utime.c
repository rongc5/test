#include <stdio.h>
#include <fcntl.h>
#include <utime.h>
#include <stdlib.h>

int main(int c, char **v)
{
    int i, fd;
    struct stat statbuf;
    struct utimbuf timebuf;

    for (i = 1; i < c; i++)
    {
        if (stat(v[i], &statbuf) < 0)
        {}
        if ((fd = open(v[i], O_RDWR | O_TRUNC)) < 0)
        {}
        close(fd);
        timebuf.actime = statbuf.st_atime;
        timebuf.modtime = statbuf.st_mtime;
        if (utime(v[i], &timebuf) < 0)
        {}
    }

    exit(0);
}
