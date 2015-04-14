#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

int main(void)
{
    int fd, ret;
    char * addr;
    struct stat buf;
    int filelen;
    int i;

    fd = open("passwd", O_RDWR);
    if (-1 == fd)
    {
        perror("open");
        exit(1);
    }

    ret = fstat(fd, &buf);
    if (-1 == ret)
    {
        perror("fstat");
        exit(1);
    }

    filelen = buf.st_size;

    addr = (char *)mmap(NULL, filelen, PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (MAP_FAILED == addr)
    {
        perror("mmap");
        exit(1);
    }

    for (i = 0; i < filelen; i++)
    {
        if (addr[i] != '\n')
            addr[i] = 'd';
        putchar(addr[i]);
    }

    munmap(addr, filelen);
    close(fd);

    return 0;
}
