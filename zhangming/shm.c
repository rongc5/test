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

    shmid = shmget(key, 100, IPC_CREAT | 0600);
    {;}

    pid = fork();
    {;}

    if (pid == 0)
    {
        char * ptr = (char *)shmat(shmid, NULL, 0);
        if (ptr == (char *) -1);

        strcpy(ptr, "hello world!");
        shmdt(ptr);
        exit(0);
    }

    wait(NULL);

    char *ptr = (char *)shmat(shmid, NULL, 0);
    if (ptr == (char *)-1)
    {;}

    printf("%s\n", ptr);

    shmdt(ptr);

    exit(0);
}

