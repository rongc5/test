#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(void)
{
    int fd;
    int ret;
    char buf[60] = {0};

    fd = open("zhangming", O_RDWR | O_APPEND);
    if (fd == -1) {
        perror("open");
        exit(1);
    }
    while (1)
        sleep(1);
#if 0
    ret = read(fd, buf, 5);
    if (ret == -1) {
        perror("read");
        exit(1);
    }
    printf("1:%s(ret=%d)\n", buf, ret);
#endif
    int sum = 0;
    while (1) {
        ret = write(fd, "abcdefghi\n", 9);
        if (ret == -1) {
            printf("sum=%d\n", sum);
            perror("write");
            exit(1);
        }
        sum += ret;
    }

#if 1
    ret = read(fd, buf, 5);
    if (ret == -1) {
        perror("read");
        exit(1);
    }
    printf("2:%sret=%d\n", buf, ret);
#endif
    close(fd);

    exit(0);
}

