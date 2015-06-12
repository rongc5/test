#include <stdio.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <fcntl.h>
#include "led.h"

int main(void)
{
    int fd, ret;
    fd =fopen("myled", O_RDWR);
    if (fd == -1) {
        perror("open");
        exit(1);
    }
    ret = ioctl(fd, LED1, OFF);
    if (-1 == ret) {
        perror("ioctl");
        exit(1);
    }

    return 0;
}
