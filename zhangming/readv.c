#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

struct tiovec
{
    void *base;
    size_t len;
};

ssize_t myreadv(int fd, struct tiovec *iov, int iovcnt)
{
    if (iovcnt <= 0 || iov == NULL)
    {
        fprintf(stderr, "usage...\n");
        return -1;
    }

    int i, ret, totallen = 0, len;
    char *buf;

    for (i = 0; i < iovcnt; i++)
    {
        buf = iov[i].base;
        len = iov[i].len;
        if (!buf || len <= 0)
        {
            fprintf(stderr, "Addr or buffer length invalied!\n");
            return -1;
        }
        ret = read(fd, buf, len);
        if (ret < len)
            iov[i].len = ret;
        totallen += ret;
        if (0 == ret)
            break;
    }
    return totallen;
}

int main(void)
{
    char buf[3][500];
    struct tiovec iov[3];
    int ret, fd;
    int i;

    iov[0].base = buf[0];
    iov[0].len = 500;
    iov[1].base = buf[1];
    iov[1].len = 500;
    iov[2].base = buf[2];
    iov[2].len = 500;

    fd = open("tmp", O_RDONLY);
    if (fd == -1)
    {
        perror("open");
        exit(1);
    }

    ret = myreadv(fd, iov, 3);
    if (-1 == ret)
        exit(1);
    for (i = 0; i < 3; i++)
    {
        if (iov[i].len > 0)
            write(1, iov[i].base, iov[i].len);
        else
            break;
    }

    printf("\nsize = %d\n", ret);
    close(fd);
    exit(0);
}
