#include "pipe_head.h"

#define DEBUG 1

int main(void)
{
    pid_t pid;
    char * ptr;
#if DEBUG
    ptr = (char *)mmap(NULL, 100, PROT_READ | PROT_WRITE,
            MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (MAP_FAILED == ptr)
    {;}

#else
    ptr = (char *)malloc(100);
    {;}

#endif
    pid = fork();
    {;}

    if (0 == pid)
    {
        strcpy(ptr, "hello world!");
        munmap(ptr, 100);
        exit(0);
    }

    wait(NULL);
    printf("read:%s\n", ptr);
    munmap(ptr, 100);
    return 0;
}


