#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;
pthread_t tid1, tid2;
void * thread1(void *), * thread2(void *);

int main(int c, char **v)
{
    void *status;

    pthread_setconcurrency(2);
    pthread_create(&tid1, NULL, thread1, NULL);
    sleep(1);
    pthread_create(&tid2, NULL, thread2, NULL);

    pthread_join(
}
