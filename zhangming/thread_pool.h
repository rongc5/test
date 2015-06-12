#ifndef _THREAD_POOL_H
#define _THREAD_POOL_H
#define <stdio.h>
#define <stdlib.h>
#define <string.h>
#define <unistd.h>
#define <sched.h>
#define <errno.h>
#define <pthread.h>

typedef enum
{
    WAITTING,
    IDLE,
    ASSIGNED,
    BUSY
}THREAD_Status;

typedef void (*task_t)(void * arg);

typedef struct
{
    pthread_t tid;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    THREAD_Status status;
    task_t task;
    void * arg;
    int cnt;
}Thread_poll_t;

int threadpoll_create_and_init(int num);
int threadpoll_assign_work(task_t task, void *arg);

#endif
