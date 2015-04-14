#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sched.h>

#define DEBUG
#ifdef DEBUG
#define PDEBUG(format, arg...) printf("QF:"format, ##arg)
#else
#define PDEBUG(format, arg...) do while(0)
#endif

typedef enum {
    WAITTING,
    IDLE,
    ASSIGNED,
    BUSY
} Thread_Status;

typedef void (*task_t)(void * arg);

typedef struct 
{
    pthread_t tid;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    Thread_Status status;
    task_t task;
    void * arg;
    int cnt;
}Thread_pool_t;

void task(void * arg)
{
    printf("I am 线程 %lu, task %d begin!\n", pthread_self(), (int)arg);
    srand(pthread_self());
    sleep(rand()%5 + 3);
    printf("task %d over!\n", (int)arg);
}

static Thread_pool_t * create_pool(int num)
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
    }
    return pools;
}

static void set_worker_status(Thread_pool_t *worker, Thread_Status status)
{
    worker->status = status;
}

static void * worker_job(void *arg)
{
    Thread_pool_t *worker = (Thread_pool_t *)arg;
    pthread_mutex_t *pmutex = &worker->mutex;
    pthread_cond_t *pcond = &worker->cond;

    while (1)
    {
        pthread_mutex_lock(pmutex);
        if (++worker->cnt == -1)
            worker->cnt = 0;
        worker->task = (task_t)0;
        worker->arg = NULL;
        set_worker_status(worker, IDLE);
        PDEBUG("thread %lu was ready!\n", pthread_self());
        pthread_cond_wait(pcond, pmutex);
        if (worker->task == (task_t)0)
        {
            pthread_mutex_unlock(pmutex);
            continue;
        }
        set_worker_status(worker, BUSY);
        pthread_mutex_unlock(pmutex);
        (worker->task)(worker->arg);
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

Thread_pool_t * Threadpool_create_and_init(int num)
{
    if (num <= 0) {
        fprintf(stderr, "Thread number invalied!\n");
        return NULL;
    }
    Thread_pool_t *polls;
    int ret;
    polls = create_pool(num);
    if (polls == NULL) 
        return NULL;
    ret = create_threads(num, polls);
    if (ret) 
        return NULL;
    wait_all_threads_ready(polls, num);

    return polls;
}

int threadPoll_assign_work(Thread_pool_t *pools, size_t poolnum, 
        task_t task, void * arg)
{
    if (!task || !pools || poolnum <= 0) {
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
        fprintf(stderr, "All threads way busy!\n");
        return -1;
    }else
    {
        pthread_mutex_lock(&pools[index].mutex);
        pools[index].task = task;
        pools[index].arg = arg;
        set_worker_status(pools + index, ASSIGNED);
        pthread_cond_signal(&pools[index].cond);
        pthread_mutex_unlock(&pools[index].mutex);
    }

    return 0;
}

int main(void)
{
    Thread_pool_t *pools;
    int i, ret, k;

    printf("请输入要创建的线程数量: ");
    fflush(stdout);
    scanf("%d", &k);
    pools = Threadpool_create_and_init(k);
    if (!pools)
        return -1;
    while (1)
    {
        printf("输入任务: ");
        fflush(stdout);
        scanf("%d", &i);
        ret = threadPoll_assign_work(pools, k, task, (void *)i);
        if (ret == -1)
            continue;
    }
}
