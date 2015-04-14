#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

int main(void)
{
    int fd, ret;
    char * addr;
    struct stat buf;
    int filelen;
    int i;

    fd = open("passwd", O_RDONLY);
    if (-1 == fd)
    {}
    ret = fstat(fd, &buf);
    if (-1 == ret)
    {}
    filelen = buf.st_size;

    addr = (char *)mmap(NULL, filelen, PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (addr == MAP_FAILED)
    {}

    for (i = 0; i < filelen; i++)
    {
        putchar(addr[i]);
        usleep(100000);
        fflush(stdout);
    }

    munmap(addr, filelen);

    close(fd);

    return 0;
}
