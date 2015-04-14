#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/sem.h>
#include <sys/ipc.h>

#define KEPATH "/etc/passwd"

typedef void (*task_t)(void *);

typedef enum
{
    IDLE, 
    RUNNING
}pro_status;

typedef struct prolib
{
    pid_t pid;
    int semid;
    int semid_index;
    task_t func;
    void *arg;
    pro_status status;
    int balance;
}Prolib_t;

int sem_init(int n)
{
     int ret;
    key = ftok();
    semid = semget(key,n,IPC_CREAT | 0600);//创造n个信号量

    union senmu st;
    arry[10]={0};
    st.array=arry;
    ret=semctl(semid,9,SETALL,st); //初始化信号量的值为0
   if(ret=-1)
   {
       perror("semctl");
       exit(1);
   }
    return semid;
}

int p(int semid, int numth)
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

    op.sem_num = numth;
    op.sem_op = 1;
    op.sem_flg = 0;

    semop(semid, &op, 1);
}

Prolib_t * process_lib_init(int n, int semid)
{
    //mmap匿名加共享方式创造Prolib_t类型结构体数组n个。
    ptr = mmap(NULL, sizeof(Prolib_t) * n,PRO_WRITEI|PRO_READ,MAP_SHARE | MAP_ANONYMOUS,-1,0);
    //初始化部分成员
    for (i = 0; i < n; i++) {
        ptr[i].status =IDLE ;
        ptr[i].balance =0 ;
        ptr[i].semid =semget(key,i,IPC_CREAT | 0600);
        ptr[i].semid_index = i;

    }
    return ptr; //返回结构体数组指针
}

static void increase_job_times(Prolib_t *worker)
{
    worker->balance++;
}

static void set_process_status(int numth, Prolib_t *lib, pro_status status)
{
    lib[numth].status = status;
}


static void child_work(int numth, Prolib_t *lib)
{
    while (1)
    {
        p(lib[numth]->semid, lib[numth]->semid_index);
        set_process_status(numth, lib, RUNNING);
        lib[numth].func(lib[numth].arg);
        increase_job_times(lib + numth);
        set_process_status(numth, lib, IDLE);
    }
}

static int creat_child_and_init_lib(int n, Prolib_t *lib,
        void(*child_func)(int numth, Prolib_t *lib))
{
    int i;

    for (i = 0; i < n; i++)
    {
    pid = fork();
    if (0 == pid)
    {
        lib[i].pid = getpid();
        lib[i].status = IDLE;
        child_func(i, lib);
        exit(0);
    }
    }
    return;
}

int creat_child_process(int n, Prolib_t *lib)
{
    int ret;
    ret = creat_child_and_init_lib(n, lib, child_work);
    return ret;
}

Prolib_t * creat_dynamic_process_libs(int n)
{
    int semid;
    Prolib_t *lib;
    semid = sem_init(n);
    lib = process_lib_init(n, semid);
    creat_child_process(n, lib);

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
                cnt == lib[i].balance;
                index = i;
            }
            else 
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

static void notify_worker(Prolib_t *lib, int numth)
{
    v(lib[numth].semid, lib[numth].semid_index);
}

static void append_job_to_worker(Prolib_t *lib, int numth,
        task_t task, void *arg)
{
    lib[numth].func = task;
}

int assign_job(Prolib_t *lib, size_t size, task_t task, void *arg)
{
    int numth;
    numth = find_idle_worker(lib, size);
    {;}
    append_job_to_worker(lib, numth, task, arg);
    notify_worker(lib, numth);
}

int main(void)
{
    Prolib *lib = creat_dynamic_process_libs(10);

    while (1)
    {

    }
}
