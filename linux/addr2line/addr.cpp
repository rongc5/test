#include "addr.h"

pthread_attr_t attr;
#define THREAD_STACK_SIZE 16*1024

void *thread_all(void *arg)
{
    thread_pool_t *worker = (thread_pool_t *)arg;
    int b= 5;

    while (1)
    {
        pthread_mutex_lock(&worker->mutex);
        while (worker->status == NO)
            pthread_cond_wait(&worker->cond, &worker->mutex);
        worker->status = NO;

        if (!worker->i){
            *(worker->buf) = 1;
        }else if (worker->i == 1){
            //strcpy(worker->date, "hello world");
            printf("hello\n");
            printf("%s\n", b);
        }else if (worker->i == 2){
            printf("%s\n", worker->i);
        }else
            printf("\n%lu i=%d hello world\n", pthread_self(), worker->i);

        pthread_mutex_unlock(&worker->mutex);
    }

}


int creat_threads(int num, thread_pool_t *pools)
{
    int i = 0, ret = 0; 
    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, THREAD_STACK_SIZE);    
    for (i; i < num; i++){
        ret = pthread_create(&pools[i].tid, &attr, thread_all, (void *)(pools + i));
        if (ret) {
            fprintf(stderr, "pthread_create:%s\n", strerror(ret));
            return ret;
        }
    }

    return 0;
}


thread_pool_t * create_pools(int thread_num)
{
    int i = 0;

    thread_pool_t * pools = (thread_pool_t *)malloc(sizeof(thread_pool_t)*thread_num);
    if (pools == NULL) {
        perror("malloc!\n");
        return NULL;
    }

    for (i; i < thread_num; i++){
        pthread_mutex_init(&pools[i].mutex, NULL);
        pthread_cond_init(&pools[i].cond, NULL);
        pools[i].index = i+1;
        pools[i].i = 0;
        pools[i].buf = NULL;
        pools[i].status = NO;
    }

    return pools;
}


int task_allocation(thread_pool_t * pools, int i, int id)
{
    pthread_mutex_lock(&pools[i].mutex);
    pools[i].status = OK;
    pools[i].i = id;
    pthread_cond_signal(&pools[i].cond);
    pthread_mutex_unlock(&pools[i].mutex);

    return 0;
}

void wait_all_threads_ready(thread_pool_t *pools, int thread_num)
{
    int i, flag;

    for (i = 0; i < thread_num; i++) {
        flag = 0;
        pthread_mutex_lock(&pools[i].mutex);
        if (pools[i].status == OK) {
            flag = 1;
        }
        pthread_mutex_unlock(&pools[i].mutex);
        if (flag)
            i--; 
    }
}

