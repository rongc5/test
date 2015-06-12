#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int lock_file(int fd, int cmd, int type, off_t offset, int whence, off_t len)
{
    struct flock lock;

    lock.l_type = type;
    lock.l_whence = whence;
    lock.l_len = len;
    lock.l_start = offset;

    return fcntl(fd, cmd, &lock);
}

int main(void)
{
    int fd, ret;
    pid_t pid;

    fd = open("tmp", O_RDWR);
    if (-1 == fd)
    {
        perror("open");
        exit(1);
    }

    pid = fork();
    if (-1 == pid)
    {
        perror("fork");
        exit(1);
    }

    if (0 == pid)
    {
        ret = lock_file(fd, F_SETLKW, F_WRLCK, 0, SEEK_SET, 5);
        if (-1 == ret)
        {
            perror("2:fcntl");
            exit(1);
        }
        printf("child lock 0 - 5 charcter!\n");
        sleep(5);
        printf("child will lock 5 - 10 charcter!\n");
        ret = lock_file(fd, F_SETLKW, F_WRLCK, 5, SEEK_SET, 5);
        if (-1 == ret)
        {
            perror("3:fcntl");
            exit(1);
        }
        printf("child lock 5 - 10 success!\n");

        return 0;
    }

    printf("father lock 5 - 10 charcter\n");
    ret = lock_file(fd, F_SETLKW, F_WRLCK, 5, SEEK_SET, 5);
        if (-1 == ret)
        {
            perror("0:fcntl");
            exit(1);
        }
        sleep(5);
    printf("father lock 0 - 5 charcter\n");
    ret = lock_file(fd, F_SETLKW, F_WRLCK, 0, SEEK_SET, 5);
        if (-1 == ret)
        {
            perror("1:fcntl");
            exit(1);
        }
        printf("father lock 0 - 5 success!\n");
        exit(0);
}
