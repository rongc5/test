#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>

#define DEBUG 0

int p(int semid)
{
    struct sembuf op;
    op.sem_num = 0;
    op.sem_op = -1;
    op.sem_flg = 0;
    int ret;

    while (1)
    {
        ret = semop(semid, &op, 1);
        if (-1 == ret)
        {
            if (errno == EINTR)
                continue;
            else 
                return -1;
        }
        else 
            break;
    }

    return 0;
}

void v(int semid)
{
    struct sembuf op;

    op.sem_num = 0;
    op.sem_op = 1;
    op.sem_flg = 0;

    semop(semid, &op, 1);
}

int main(void)
{
    key_t key;
    int ret, semid;
    pid_t pid;
    int i;
    
    key = IPC_PRIVATE;

    semid = semget(key, 1, IPC_CREAT | 0600);
    {}

    ret = semctl(semid, 0, SETVAL, 3);
    {}
//  int loop = 3;
    for (i = 0; i < 100; i++)
    {
#if DEBUG      
        if (0 == loop)
        {
            i--;
            continue;
        }
        loop--;
#endif
        p(semid);
        pid = fork();
        {}

        if (0 == pid)
        {
            printf("pid %d will run!\n", getpid());
            srand(getpid());
            sleep(rand()%5 + 1);
            printf("pid %d run over!\n", getpid());
            //loop++;
            v(semid);
            exit(0);
        }
    }

    exit(0);
}
