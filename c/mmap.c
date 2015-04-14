#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(void)
{
    int fd = open("a", O_RDWR,00777);
    if (fd == -1){
        perror("open");
        return -1;
    }

    struct stat statbuf;
    int err = stat("a", &statbuf);
    if (err == -1){
        perror("stat...");
        return -1;;
    }

    char *p_share = (char*)mmap(NULL,statbuf.st_size, PROT_READ,MAP_PRIVATE,fd,0);
    if (MAP_FAILED == p_share) {
        perror("mmap");
        return -1;
    }

    return 0;
}
