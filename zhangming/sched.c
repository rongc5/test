#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>

typedef unsigned long long int ull_t;

void display(struct timeval * prev, struct timeval * next)
{
    suseconds_t usec;
    time_t sec;
    unsigned long msec;

    sec = next->tv_sec - prev->tv_sec;
    usec = next->tv_usec - prev->tv_usec;

    usec += sec * 1000000;
    msec += usec / 1000;
    printf("(%lu) was consumed\n", pthread_self(), msec);
}

void do_prime(void)
{
    struct timeval prev, next;
    ull_t i;
    int flag = 0;
    ull_t base = 30000000ull;

    gettimeofday(&prev, NULL);
    for (i = 2; i < base; i++)
    {
        if (base % i == 0)
        {
            flag = 1;
            break;
        }
    }
    gettimeofday(&next, NULL);
    display(&prev, &next);
}

void * thread_handler(void *arg)
{
    struct sched_param param;
    int policy;
    pthread_t tid;

    tid = pthread_self();

    pthread_getschedparam(tid, &policy, &param);
    if (policy == SCHED_OTHER)
        printf("other!\n");
    else if (policy == SCHED_FIFO)
        printf("fifo!\n");
    else if (policy == SCHED_RR)
        printf("rr!\n");
    printf("priority:%d\n", param.sched_priority);
    
    do_prime();
    return NULL;
}

int main(void)
{
    pthread_t tid;
    int ret;
    pthread_attr_t attr;
    int policy;
    struct sched_param param;

    policy = SCHED_FIFO;
    param.sched_priority = 99;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

#if 1
    //!!!!!!
    pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    //!!!!!!!!!!!!
    ret = pthread_attr_setschedpolicy(&attr, policy);
    if (ret)
    {
        fprintf(stderr, "param:%s\n", strerror(ret));
        exit(1);
    }
#endif
    ret = pthread_create(&tid, &attr, thread_handler, NULL);
    if (ret)
    {
        fprintf(stderr, "pthread_create!:%s", strerror(ret));
        exit(1);
    }
    printf("%lu was created!\n", tid);
    do_prime();

    pthread_exit(NULL);
}
