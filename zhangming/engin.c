#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

typedef void (*task_t)(void *);

typedef enum
{
    UNPROCEED,
    IDLE,
    RUNNING
}pro_status;

typedef struct prolib
{
    pid_t pid;
    int semid;
    int semid_index;
    int shmid;
    task_t func;
    void *arg;
    size_t argsize;
    pro_status status;
    int balance;
}Prolib_t;

Prolib_t * create_dynamic_process_libs(int n);

int assign_job(Prolib_t *lib, size_t libsize, task_t task, void *arg, size_t argsize);

int sem_init(int n)
{
    key_t key;
    int ret;
    unsigned short array[n];
    int semid;
    
    memset(array, 0, sizeof(short)*n);
    key = IPC_PRIVATE;
    semid = semget(key, n, IPC_CREAT | 0600);
    if (-1 == semid)
    {
        perror("semget");
        return -1;
    }

    ret = semctl(semid, 0, SETALL, array);
    if (-1 == ret)
    {
        perror("semctl");
        return -1;
    }

    return semid;
}

void p(int semid, int numth)
{
    struct sembuf op;
    int ret;

    op.sem_num = numth;
    op.sem_op = -1;
    op.sem_flg = 0;
    while (1)
    {
        ret = semop(semid, &op, 1);
        if (-1 == ret)
        {
            perror("semop");
            continue;
        }else 
            break;
    }
}

void v(int semid, int numth)
{
    struct sembuf op;

    op.sem_num = numth;
    op.sem_op = 1;
    op.sem_flg = 0;

    semop(semid, &op, 1);
}

Prolib_t * process_lib_init(int n, int semid)
{
    Prolib_t * ptr;
    int i, size;

    size = sizeof(Prolib_t)*n;

    ptr = (Prolib_t *)mmap(NULL, size, PROT_READ | PROT_WRITE,
            MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (ptr == MAP_FAILED)
    {
        perror("mmap");
        return NULL;
    }

    for (i = 0; i < n; i++)
    {
        ptr[i].status = UNPROCEED;
        ptr[i].balance = 0;
        ptr[i].semid = semid;
        ptr[i].semid_index = i;
        ptr[i].func = NULL;
        ptr[i].argsize = 0;
    }

    return ptr;
}

static void increase_job_times(Prolib_t *worker)
{
    worker->balance++;
}

static void *set_process_status(int numth, Prolib_t *lib, pro_status status)
{
    lib[numth].status = status;
}

static void * get_arg(Prolib_t *lib, int numth)
{
    if (lib[numth].shmid == -1)
        return NULL;
}

static void destroy_shared_ipc(int shmid, void *addr)
{
    if (addr)
        shmdt(addr);
    if (shmid != -1)
    shmctl(shmid, IPC_RMID, NULL);
}

static void child_work(int numth, Prolib_t *lib)
{
    while (1)
    {
        set_process_status(numth, lib, IDLE);
        p(lib[numth].semid, lib[numth].semid_index);
        set_process_status(numth, lib, RUNNING);
        lib[numth].arg = get_arg(lib, numth);
        lib[numth].func(lib[numth].arg);
        destroy_shared_ipc(lib[numth].semid, lib[numth].arg);
        increase_job_times(lib + numth);
    }
}

static int create_child_and_init_lib(int n, Prolib_t *lib, 
        void (*child_func)(int numth, Prolib_t *lib))
{
    int i;
    pid_t pid;
    for (i = 0; i < n; i++)
    {
        pid = fork();
        if (-1 == pid)
        {
            perror("fork");
            return -1;
        }

        if (pid = 0)
        {
            lib[i].pid = getpid();
            child_func(i, lib);
            exit(0);
        }
    }

    return 0;
}

int create_child_process(int n, Prolib_t *lib)
{
    int ret;
    ret = create_child_and_init_lib(n, lib, child_work);
    return ret;
}

static void wait_child_ready(Prolib_t * lib, size_t n)
{
    int i;

    for (i = 0; i < n; i++)
    {
        if (lib[i].status == UNPROCEED)
            i--;
    }
}

Prolib_t * create_dynamic_process_libs(int n)
{
    int semid;
    Prolib_t *lib;
    int ret;

    semid = sem_init(n);
    if (-1 == semid)
        return NULL;

    lib = process_lib_init(n, semid);
    if (lib == NULL)
        return NULL;

    ret = create_child_process(n, lib);
    if (-1 == ret)
        return NULL;

    wait_child_ready(lib, n);
    return lib;
}

static int find_idle_worker(Prolib_t *lib, size_t size)
{
    int ret, i;
    int index = -1, cnt = -1;
    for (i = 0; i < size; i++)
    {
        if (lib[i].status == IDLE)
        {
            if (-1 == cnt)
            {
                cnt = lib[i].balance;
                index = i;
            }else
            {
                if (cnt > lib[i].balance)
                {
                    cnt = lib[i].balance;
                    index = i;
                }
            }
        }
    }
    return index;
}

static void notify_worker(Prolib_t * lib, int numth)
{
    v(lib[numth].semid, lib[numth].semid_index);
}

static void copy_arg_to_worker(Prolib_t *worker, void *arg, size_t argsize)
{
    char * ptr;
    int shmid;

    shmid = shmget(IPC_PRIVATE, argsize, IPC_CREAT | 0600);
    if (-1 == shmid)
    {
        perror("shmget");
        return;
    }
    ptr = (char *)shmat(shmid, NULL, 0);
    if (ptr == (void *)-1)
    {
        perror("shmat");
        return;
    }
    worker->shmid = shmid;
    memcpy(ptr, arg, argsize);
    shmdt(ptr);
    worker->argsize = argsize;
}

static void append_job_to_worker(Prolib_t *lib, int numth, task_t task,
        void *arg, size_t argsize)
{
    lib[numth].func = task;
    if (arg != NULL && argsize > 0)
        copy_arg_to_worker(lib + numth, arg, argsize);
    else 
    {
        lib[numth].argsize = -1;
        lib[numth].shmid = -1;
    }
}

int assign_job(Prolib_t *lib, size_t libsize, task_t task, void *arg, size_t argsize)
{
    int numth;
    numth = find_idle_worker(lib, libsize);
    if (-1 == numth)
    {
        fprintf(stderr, "All process are busy!\n");
        return -1;
    }
    append_job_to_worker(lib, numth, task, arg, argsize);
    notify_worker(lib, numth);
}

void task1(void *arg)
{
    printf("task1, arg = %d!\n", *((int *)arg));
    sleep(5);
    printf("task1 end!\n");
}
void task2(void *arg)
{
    printf("task2, arg = %d!\n", *((int *)arg));
    sleep(5);
    printf("task2 end!\n");
}

void task3(void *arg)
{
    printf("task3, arg = %d!\n", *((int *)arg));
    sleep(5);
    printf("task3 end!\n");
}

void task4(void *arg)
{
    printf("task4, arg = %d!\n", *((int *)arg));
    sleep(5);
    printf("task4 end!\n");
}

int main(void)
{
    int op, ret;
    Prolib_t *lib = create_dynamic_process_libs(3);
    if (lib == NULL)
        exit(1);

    while (1)
    {
        printf("Input task: ");
        fflush(stdout);
        scanf("%d", &op);
        if (1 == op)
            ret = assign_job(lib, 3, task1, &op, sizeof(int));
        else if (2 == op)
            ret = assign_job(lib, 3, task2, &op, sizeof(int));
        else if (3 == op)
            ret = assign_job(lib, 3, task3, NULL, 0);
        else if (4 == op)
            ret = assign_job(lib, 3, task4, NULL, 0);
        if (ret == -1)
            continue;
    }
}
