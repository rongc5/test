#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sched.h>

pthread_mutex_t que_lock;
#define DEBUG
#ifdef DEBUG
#define PDEBUG(format, arg...) printf("QF:"format, ##arg)
#else
#define PDEBUG(format, arg...) do while(0)
#endif

typedef char elmType;
typedef void (*task_t)(void *);
#define ELMSIZE 65536

typedef enum
{
    EMPTY,
    NORMAL,
    FULL
}Que_status_t;

typedef enum
{
    WAITTING,
    IDLE,
    ASSIGNED,
    BUSY
}Thread_status;

typedef struct que
{
    elmType *buf;
    int head;
    int tail;
    Que_status_t status;
}Que_t;

typedef struct
{
    pthread_t tid;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    Thread_status status;
    task_t task;
    void * arg;
    int cnt;
    Que_t * taskque;
}Thread_pool_t;


typedef struct task_comp
{
    task_t task;
    void *arg;
}task_comp_t;

Que_t *create_task_que(size_t tasksize);
Que_t * create_and_init_que(size_t elemsize)
{
    Que_t * qp = (Que_t *)malloc(sizeof(Que_t));
    if (!qp) {
        perror("malloc");
        return NULL;
    }

    qp->buf = (char *)malloc(elemsize * ELMSIZE);
    if (!qp->buf) {
        perror("malloc");
        return NULL;
    }

    qp->head = qp->tail = 0;
    qp->status = EMPTY;

    return qp;
}

void mycpy(char *dst, char *src, size_t size)
{
    while (size--) 
        *(dst++) = *(src++);
}

int read_one_elm(Que_t *qp, void *ep, size_t elemsize)
{
    if (qp->status == EMPTY)
        return -1;
    mycpy((char *)ep, (char *)(qp->buf + qp->tail*elemsize), elemsize);
    qp->tail++;
    if (qp->tail == ELMSIZE)
        qp->tail = 0;
    if (qp->head == qp->tail)
        qp->status = EMPTY;
    else
        qp->status = NORMAL;
    return 0;
}

int read_que(Que_t *qp, void *buf, size_t size, size_t elemsize)
{
    int i, ret;
    char *base = (char *)buf;

    for (i = 0; i < size; i++)
    {
        ret = read_one_elm(qp, base+i*elemsize, elemsize);
        if (ret == -1)
            break;
    }

    return i;
}

int write_one_elm(Que_t *qp, void *ep, size_t elemsize)
{
    if (qp->status == FULL)
        return -1;
    mycpy((char *)(qp->buf + qp->head * elemsize), 
            (char *)ep, elemsize);
    qp->head++;
    if (qp->head == ELMSIZE)
        qp->head = 0;
    if (qp->head == qp->tail)
        qp->status = FULL;
    else
        qp->status = NORMAL;
    return 0;
}

int write_que(Que_t *qp, const void *buf, size_t size, size_t elemsize)
{
    int i, ret;
    char *base = (char *)buf;

    for (i = 0; i < size; i++) {
        ret = write_one_elm(qp, base + i*elemsize, elemsize);
        if (ret == -1)
            break;
    }

    return i;
}

int que_is_empty(Que_t *qp)
{
    return qp->status == EMPTY ? 1:0;
}

static Thread_pool_t * create_pool(int num, Que_t *qp)
{
    int i;
    Thread_pool_t *pools = (Thread_pool_t *)malloc(sizeof(Thread_pool_t)*num);
    if (!pools) {
        perror("malloc");
        return NULL;
    }

    for (i = 0; i < num; i++)
    {
        pthread_mutex_init(&pools[i].mutex, NULL);
        pthread_cond_init(&pools[i].cond, NULL);
        pools[i].status = WAITTING;
        pools[i].task = (task_t)(0);
        pools[i].arg = NULL;
        pools[i].cnt = -1;
        pools[i].taskque = qp;
    }
    return pools;
}

static void set_worker_status(Thread_pool_t *worker, Thread_status status)
{
    worker->status = status;
}

static void * worker_job(void *arg)
{
    Thread_pool_t *worker = (Thread_pool_t *)arg;
    pthread_mutex_t *pmutx = &worker->mutex;
    pthread_cond_t *pcond = &worker->cond;
    task_comp_t task;
    int times =0;

    while (1)
    {
        pthread_mutex_lock(pmutx);
        if (-1 == (worker->cnt += times))
            worker->cnt = 0;
        worker->task = (task_t)0;
        worker->arg = NULL;
        set_worker_status(worker, IDLE);
        PDEBUG("thread %lu was ready!\n", pthread_self());
        pthread_cond_wait(pcond, pmutx);
        if (worker->task == (task_t)0)
        {
            pthread_mutex_unlock(pmutx);
            continue;
        }
        PDEBUG("thread %lu was waken!\n", pthread_self());
        set_worker_status(worker, BUSY);
        pthread_mutex_unlock(pmutx);
        (worker->task)(worker->arg);
        times++;
        while (1)
        {
            if (!taskque_is_empty(worker->taskque))
            {
                printf("task excute!\n");
                remove_one_task(worker->taskque, &task);
                printf("task begin excute!, task(%p), arg(%p)\n", 
                        task.task, task.arg);
                (task.task)(task.arg);
                times++;
            }else
                break;
        }
    }

    return NULL;
}


static int create_threads(int num, Thread_pool_t *pools)
{
    int i, ret;

    for (i = 0; i < num; i++)
    {
        ret = pthread_create(&pools[i].tid, NULL, worker_job, (void *)(pools + i));
        if (ret)
        {
            fprintf(stderr, "pthread_create:%s\n", strerror(ret));
            return ret;
        }
    }
    return ret;
}

static void wait_all_threads_ready(Thread_pool_t *pools, int num)
{
    int i;

    for (i = 0; i < num; i++)
    {
        while (1)
        {
            pthread_mutex_lock(&pools[i].mutex);
            if (pools[i].status == IDLE)
            {
                pthread_mutex_unlock(&pools[i].mutex);
                break;
            }
            else 
            {
                pthread_mutex_unlock(&pools[i].mutex);
                sched_yield();
            }
        }
    }
}

Thread_pool_t * threadPool_create_and_init(int num)
{
    if (num <= 0)
    {
        fprintf(stderr, "Thread number inivalied!\n");
        return NULL;
    }
    Thread_pool_t *polls;
    int ret;
    Que_t *qp;

    qp = create_task_que(sizeof(task_comp_t));
    if (!qp)
        return NULL;

    polls = create_pool(num, qp);
    if (polls == NULL) 
        return NULL;

    ret = create_threads(num, polls);
    if (ret) 
        return NULL;

    wait_all_threads_ready(polls, num);

    return polls;
}

int thread_assign_work(Thread_pool_t *pools, size_t poolnum, task_t task, void * arg)
{
    if (!task || !pools || poolnum <= 0)
    {
        fprintf(stderr, "argument ivalied!\n");
        return -1;
    }

    int i, index = -1, cnt = -1;

    for (i = 0; i < poolnum; i++)
    {
        pthread_mutex_lock(&pools[i].mutex);
        if (pools[i].status == IDLE)
        {
            if (-1 == index)
            {
                index = i;
                cnt = pools[i].cnt;
            }else
            {
                if (cnt > pools[i].cnt)
                {
                    index = i;
                    cnt = pools[i].cnt;
                }
            }
        }
        pthread_mutex_unlock(&pools[i].mutex);
    }

    if (-1 == index)
    {
        task_comp_t tk;
        tk.task = task;
        tk.arg = arg;
        printf("task append!, task(%p), arg(%p)\n", task, arg);
        if (-1 == append_one_task(pools[0].taskque, &tk))
        {
            fprintf(stderr, "task was full, system are too busy!!\n");
            return -1;
        }
    }else
    {
        pthread_mutex_lock(&pools[index].mutex);
        pools[index].task = task;
        pools[index].arg = arg;
        set_worker_status(pools + index, ASSIGNED);
        PDEBUG("thread %lu was signal!\n", pools[index].tid);
        pthread_cond_signal(&pools[index].cond);
        pthread_mutex_unlock(&pools[index].mutex);
    }

    return 0;
}

Que_t *create_task_que(size_t tasksize)
{
    if (tasksize <= 0)
    {
        fprintf(stderr, "Invalied argument!\n");
        return NULL;
    }
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP);
    pthread_mutex_init(&que_lock, &attr);
    pthread_mutexattr_destroy(&attr);

    return create_and_init_que(tasksize);
}

int append_one_task(Que_t *qp, const task_comp_t *task)
{
    int ret;

    pthread_mutex_lock(&que_lock);
    ret = write_que(qp, task, 1, sizeof(task_comp_t));
    return ret;
}

int remove_one_task(Que_t *qp, task_comp_t *task)
{
    int ret;
    pthread_mutex_lock(&que_lock);
    ret = read_que(qp, task, 1, sizeof(task_comp_t));
    pthread_mutex_unlock(&que_lock);
    return ret;
}

int taskque_is_empty(Que_t *qp)
{
    int ret;
    pthread_mutex_lock(&que_lock);
    ret = que_is_empty(qp);
    pthread_mutex_unlock(&que_lock);
    return ret;
}

void task(void *arg)
{
    printf("task %d begin!\n", (int)arg);
    srand(pthread_self());
    sleep(rand()%5+5);
    printf("task %d over!\n", (int)arg);
}

int main(void)
{
    Thread_pool_t *pools;
    int i, ret;

    pools = threadPool_create_and_init(9);
    if (!pools)
        return -1;
    while (1)
    {
        printf("Input task : ");
        fflush(stdout);
        scanf("%d", &i);
        ret = thread_assign_work(pools, 9, task, (void *)i);
        if (ret == -1) 
            continue;
    }
}
