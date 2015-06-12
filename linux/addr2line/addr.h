#ifndef __ADDR_H_
#define __ADDR_H_


#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/un.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <netinet/tcp.h>
#include <netinet/in.h>


#ifdef __DEBUG_HAIER_PRINT_
#define DEBUG printf("In file: %s function: %s line: %d\n", __FILE__\
        , __func__, __LINE__)
#endif


typedef enum
{
    NO,
    OK
}Status_t;

typedef struct
{
    pthread_t tid;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int index;
    int i;
    char *buf;
    char date[12];
    Status_t status;
}thread_pool_t;


void *thread_all(void *arg);


int creat_threads(int num, thread_pool_t *pools);


thread_pool_t * create_pools(int thread_num);


int task_allocation(thread_pool_t * pools, int thread_num, int id);

void wait_all_threads_ready(thread_pool_t *pools, int thread_num);

#endif
