#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <string.h>
#include <pthread.h>
#include <sched.h>

#define SERVERPATH "/tmp/serverend"

typedef enum
{
    WAITTING,
    IDLE,
    ASSIGNED,
    BUSY
}Thread_status;

typedef void (*task_t)(void * arg);

typedef struct
{
    pthread_t tid;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    Thread_status status;
    task_t task;
    void * arg;
    int cnt;
}Thread_pool_t;

void task(void * arg)
{
    int ret;
    char buf[512];

    printf("task is excuted!\n");
    int newsd = (int)arg;
    ret = read(newsd, buf, 512);
    if (ret == -1) {
        perror("read");
        return;
    }
    if (ret == 0) {
        fprintf(stderr, "end of file!\n");
        return;
    } else if (ret > 0) {
        buf[ret] = '\0';
        printf("From client:%s\n", buf);
        write(newsd, "hello world", 11);
    }
    sleep(3);
    close(newsd);
}

static Thread_pool_t * create_pool(int num)
{
    int i;
    Thread_pool_t *pools = (Thread_pool_t *)malloc(sizeof(Thread_pool_t) * num);
    if (!pools) 
    {
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

static void set_worker_status(Thread_pool_t *worker, Thread_status status)
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
        printf("thread %lu was ready!\n", pthread_self()); 
        pthread_cond_wait(pcond, pmutex);
        pthread_cond_wait(pcond, pmutex);
        if (worker->task == (task_t)0) {
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
            printf("I am tid %llu ready\n", (unsigned long long)pthread_self());
        }
    }
}

Thread_pool_t * threadPool_create_and_init(int num)
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

int threadPoll_assign_work(Thread_pool_t *pools, size_t poolnum, task_t task,void * arg)
{
    if (!task || !pools || poolnum <= 0) {
        fprintf(stderr, "argument ivalied!\n");
        return -1;
    }    

    int i, index = -1, cnt = -1;
    for (i = 0; i < poolnum; i++) 
    {
        pthread_mutex_lock(&pools[i].mutex);
        if (pools[i].status == IDLE) {
            if (index == -1) {
                index = i;
                cnt = pools[i].cnt;
            } else {
                if (cnt > pools[i].cnt) {
                    index = i;
                    cnt = pools[i].cnt;
                }
            }
        }
        pthread_mutex_unlock(&pools[i].mutex);
    }

    if (-1 == index)
    {
        fprintf(stderr, "All threads was busy!\n");
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
    int sd, ret, newsd, k;
    struct sockaddr_un serverend;
    socklen_t len;
    char buf[512];
    Thread_pool_t *pools;

    len = sizeof(serverend);
    serverend.sun_family = AF_UNIX;
    strcpy(serverend.sun_path, SERVERPATH);

    daemon(1, 1);
    printf("输入要创建的子进程数目: ");
    fflush(stdout);
    scanf("%d", &k);
    pools = threadPool_create_and_init(k);
    if (!pools)
        return -1;

    sd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sd == -1) {
        perror("socket");
        exit(1);
    }

    unlink(SERVERPATH);
    ret = bind(sd, (struct sockaddr *)&serverend, len);
    if (ret == -1) {
        perror("bind");
        exit(1);
    }

    listen(sd, 10);

    while (1)
    {
        newsd = accept(sd, NULL, NULL);
        printf("before assign work!\n");
        threadPoll_assign_work(pools, 5, task, (void *)newsd);
    }
}
