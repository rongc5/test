#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>

int main(void)
{
    key_t key = IPC_PRIVATE;
    int shmid;
    int size = 100;
    pid_t pid;

    shmid = shmget(key, 100, IPC_PRIVATE | 0600);
    if (-1 == shmid)
    {
        perror("shmget");
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
        char * ptr = (char *)shmat(shmid, NULL, 0);
        if (ptr == (char *)-1)
        {
            perror("shmat");
            exit(1);
        }

        strncpy(ptr, "hello world!", 13);
        shmdt(ptr);
        exit(0);
    }
    wait(NULL);

    char *ptr = (char *)shmat(shmid, NULL, 0);
    if (ptr == (char *)-1)
    {
        perror("shmat");
        exit(1);
    }

    printf("%s\n", ptr);

    shmdt(ptr);

    exit(0);
}
