#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

struct iovec
{
    void *iov_base;
    size_t iovlen;
};

ssize_t myreadv(int fd, const struct iovec *iov, int iovcnt)
{
    int ret, rd;
    rd = (*iov.iovlen)*iovcnt;
    char buf[rd];

    ret = read(fd, buf, rd);
    if (ret == -1)
    {
        perror("read");
        exit(1);
    }
    if (buf[ret] == '\n')
        buf[ret] = '\0';
    
    iov->iov_base = buf;
    
    
    return ret;
}

int main(void)
{
    int fd;
    char len = 256;
    char buf[len]; 
    int iovcnt = 5;
    struct iovc *tm;
    tm->iov_base = buf;
    tm->iovlen = iovcnt;
    
    fd = open("passwd", O_RDONLY);
    if (-1 == fd)
    {
        perror("open");
        exit(1);
    }
    
    myreadv(fd, tm, iovcnt);

    close(fd);

    return 0;
}

