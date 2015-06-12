#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

int fd;
int g_lock = 0;
//pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;

void mutex_lock(int *lock)
{
    while (1)
    {
        if (*lock == 1)
        {
            usleep(1000);
            continue;
        }
        else if (*lock == 0)
        {
            *lock = 1;
            break;
        }
    }
}

void mutex_unlock(int *lock)
{
    if (*lock == 1)
        *lock = 0;
}

void * thread_handler(void * arg)
{
    pthread_detach(pthread_self());

    int op;
    unsigned long long consume = 999999999;
    op = (int)arg;
    fd = 1;
    int lock = 0;

    if (op == 1)
        sleep(5);
//    mutex_lock(&lock);
    //pthread_mutex_lock(&g_mutex);
    if (0 == op)
        write(fd, "    A\n", 6);
    else
        write(fd, "    C\n", 6);
 //   while (consume--);
   // sleep(3);
    if (0 == op)
        write(fd, "    B\n", 6);
    else
        write(fd, "    D\n", 6);
    sync();
  //  mutex_unlock(&lock);
    //pthread_mutex_lock(&g_mutex);
    pthread_exit(NULL);
}

int main(void)
{
    int ret, i;
    pthread_t tid[2];

    fd = open("tmp", O_RDWR);
    if (-1 == fd)
    {
        perror("open");
        exit(1);
    }

//    pthread_mutex_init(&g_mutex, NULL);
    
    for (i = 0; i < 2; i++)
    {
        ret = pthread_create(tid + i, NULL, thread_handler, (void *)i);
        if (ret)
        {
            fprintf(stderr, "pthread:%s\n", strerror(ret));
            exit(1);
        }
    }
    pthread_exit(NULL);
}
