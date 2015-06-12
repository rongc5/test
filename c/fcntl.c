#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

       off_t lseek(int fd, off_t offset, int whence);


       int fcntl(int fd, int cmd, ... /* arg */ );

int main(int c, char **v)
{
    int fd = open("tmp",  O_RDWR | O_CREAT, 0600);
    if (-1 == fd) {
        perror("open");
        return 1;
    }
    
    struct flock lock;

    lock.l_type = F_WRLCK;
    lock.l_start = 0;
    lock.l_whence = SEEK_SET;
    lock.l_len = 0;

    fcntl(fd, F_SETLKW, &lock);

    lseek(fd, 0, SEEK_END);
    printf("I am get the lock!\n");
    write(fd, "hello world", 13);

    sleep(30);
    
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLKW, &lock);
    

    return 0;

}
