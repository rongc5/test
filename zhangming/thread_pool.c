#include "thread_pool.h"

int g_val = 0;
pthread_mutex_t g_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t g_cond = PTHREAD_COND_INITIALIZER;

static thread_poll_t * pool = NULL;

void do_prime(int s)
{
    ull_t i;
    int flag = 0;

    for (i = 2; i < s+BASE; i++) {
        if ((s+BASE) % i == 0) {
            flag = 1;
            break;
        }
    }

    if (flag == 1)
        printf("\n%llu is not a prime!\n", (ull_t)(s+BASE));
    else
        printf("\n%llu is a prime!\n", (ull_t)(s+BASE));
}

int threadpoll_creat_and_init(int num)
{
    int ret, i;
    pthread_t tid;
    
    pool = (thread_poll_t *)malloc(sizeof(thread_poll_t));
    pthread_mutex_init(&mutex, NULL);

    do
    {
        ret = pthread_create(&tid, NULL, thread_handl, NULL);
        if (ret)
        {
            fprintf(stderr, "pthread:%s", strerror(ret));
            return -1;
        }
    }while (i < num);

    return 0;
}

int threadpoll_assign_work(task_t task, void * arg)
{
    
}

static void wait_child_ready(thread_poll_t * tpt, size_t n)
{
    int i;

    for (i = 0; i < n; i++)
    {
        if (lib[i].status == WAITTING)
            i--;
    }
}

thread_poll_t  thread_lib_init(int n)
{
    thread_poll_t ptr[n];
    int i;

    for (i = 0; i < n; i++)
    {
        ptr[i].tid = pthread_self();
        ptr[i].mutex = PTHREAD_MUTEX_INITIALIZER;
        ptr[i].cond = PTHREAD_COND_INITIALIZER;
        ptr[i].status = WAITTING;
        ptr[i].task = NULL;
    }
}

void * thread_handler(void * arg)
{
    int tmp;

    while (1)
    {
        pthread_mutex_lock(&g_lock);
        printf("tid %lu wait!\n", pthread_self());
        pthread_cond_wait(&g_cond, &g_lock);
        tmp = g_val;
        printf("I am %lu!\n", pthread_self());
        pthread_mutex_unlock(&g_lock);
        do_prime(tmp);
    }
}

int main(void)
{
    int k;
    
    printf("输入创建的线程数量: ");
    scanf("%d", &k);
    threadpoll_creat_and_init(k);

    sleep(1);
    while (1) {
        printf("Please input number: ");
        fflush(stdout);
        scanf("%d", &k);
        threadpoll_assign_work(k, NULL);
    }
    return 0;
}





