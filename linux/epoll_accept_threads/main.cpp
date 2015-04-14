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


#define SERVER_PORT 7881
#define SERVER_IP 	  "192.168.3.112"


#ifdef __DEBUG_HAIER_PRINT_
#define DEBUG printf("In file: %s function: %s line: %d\n", __FILE__\
        , __func__, __LINE__)
#endif


#define MAX_RLIMIT_NOFILE 10000
#define THREADNUM   1

pthread_attr_t attr;
#define THREAD_STACK_SIZE 16*1024
typedef :wq



typedef enum
{
    WAITTING,
    IDLE,
    BUSY
}Status_t;

typedef struct
{
    pthread_t tid;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int index;
    int sfd;
    Status_t status;
}thread_pool_t;


void displayTimeUsed(struct timeval * prev)
{   
    struct timeval next;
    suseconds_t usec;
    time_t sec;
    unsigned long msec;    
    float used;

    gettimeofday(&next, NULL);
    sec = next.tv_sec - prev->tv_sec;
    usec = next.tv_usec - prev->tv_usec;
    used = sec + usec*1.0/1000000;
    printf("thread_id:(%lu)  %0.3f (seconds)was consumed\n", pthread_self(), used);
}

int SetSockFdAttr(int SockFd)
{
    int bReuseAddr = 1;
    struct linger OptLinger;

    struct timeval timeout;
    timeout.tv_sec=10;
    timeout.tv_usec=0;

    OptLinger.l_onoff = 1;
    OptLinger.l_linger = 1;

    setsockopt(SockFd, SOL_SOCKET, SO_REUSEADDR, &bReuseAddr, sizeof(bReuseAddr));
    setsockopt(SockFd, SOL_SOCKET, SO_LINGER, &OptLinger, sizeof(OptLinger));
    setsockopt(SockFd, IPPROTO_TCP, TCP_NODELAY, &bReuseAddr, sizeof(bReuseAddr));
    //setsockopt (SockFd, SOL_TCP, TCP_CORK, &OptLinger, sizeof(OptLinger));

    setsockopt(SockFd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(struct timeval));
    setsockopt(SockFd, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(struct timeval));

    return 0;
}

int set_rlimit(int resource, size_t size)
{
    struct rlimit limit;

    if (getrlimit(resource, &limit) != 0)
    {
        DEBUG;
        return 1;
    }

    if (limit.rlim_cur == RLIM_INFINITY || (size != RLIM_INFINITY && limit.rlim_cur >= size))
    {
        return 0;
    }

    limit.rlim_cur = size;
    limit.rlim_max = size;
    if (setrlimit(resource, &limit) != 0)
    {
        return 1;
    }

    setrlimit(resource, &limit);
}

static int proc_ignore_sigpipe(void)
{
    struct sigaction act;
    memset(&act, 0, sizeof(act));
    sigemptyset(&act.sa_mask);
    act.sa_handler = SIG_IGN;
    act.sa_flags = 0;
    if(sigaction(SIGPIPE, &act, NULL) < 0){
        DEBUG;
        return 1;
    }
    return 0;
}

void *thread_all(void *arg)
{
    struct sockaddr_in serveraddr;
    serveraddr.sin_port = htons(SERVER_PORT);
    serveraddr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, &serveraddr.sin_addr.s_addr);


    int i=0;
    int sfd;
    struct sockaddr_in sa;
    socklen_t len = sizeof(struct sockaddr);

    pthread_mutex_lock(&worker->mutex);
    while (worker->status == NO)
        pthread_cond_wait(&worker->cond, &worker->mutex);
    worker->status = NO;
    memcpy(MACSTR, worker->MACSTR, strlen(worker->MACSTR));
    pthread_mutex_unlock(&worker->mutex);

    while (1)
    {
        gettimeofday(&prev, NULL);
        displayTimeUsed(&prev);	

        exit(0);
    }

}

static int creat_threads(int num, thread_pool_t *pools)
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

static thread_pool_t * create_pools(int thread_num)
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
        pools[i].status = NO;
    }

    return pools;
}

static int task_allocation(thread_pool_t * pools, int thread_num, int sfd)
{
    int i;


    for (i = 0; i < thread_num; i++) {
        pthread_mutex_lock(&pools[i].mutex);
        if (pools[i].status == IDLE){
            pools[i].sfd = sfd;
            pthread_cond_signal(&pools[i].cond);
            break;
        }
            pthread_mutex_unlock(&pools[i].mutex);
    }
    return 0;
}

static void wait_all_threads_ready(thread_pool_t *pools, int thread_num)
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

int init_listener(){
    int listener, ret;
    if ((listener = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        logError("file: "__FILE__", line: %d , " \
                "can not get listener fd", __LINE__);
        DEBUG;
        return -1;
    }
    /*
       if((ret = set_nonblock(listener)) != 0){
       logError("file: "__FILE__", line: %d , " \
       "can not set nonblock to listener fd, errno:%d, error " \
       "info: %s", __LINE__,ret, strerror(ret));
       return -1;
       }
       */
    //set_nonblock(listener);
    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    unsigned int      optval = 0x1;  
    struct linger     optlinger;


    setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &optval, 4);
#if 1
    optlinger.l_onoff = 1;  
    optlinger.l_linger = 5;  
    setsockopt(listener, SOL_SOCKET, SO_LINGER, &optlinger, sizeof(struct linger));
#endif
    //int val = 3;
    //setsockopt(listener, SOL_TCP, TCP_DEFER_ACCEPT, &val, sizeof(val)); 

    //setsockbuff(listener);
#if 0
    struct timeval timeout;
    timeout.tv_sec=3; // 2 second
    timeout.tv_usec=0;

    setsockopt(listener, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(struct timeval));
    setsockopt(listener, SOL_SOCKET,SO_SNDTIMEO, (char *)&timeout, sizeof(struct timeval));
#endif

    //int val = 1;
    //setsockopt(infoptr->fd, IPPROTO_TCP, TCP_NODELAY, &val, sizeof(val));
    //setsockopt (infoptr->fd, SOL_TCP, TCP_CORK, &val, sizeof (val));


    if( bind(listener, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {  
        close(listener); 
        DEBUG;
        return(-1);  
    }

    if ( listen(listener, MAX_LINK) < 0) {  
        close(listener); 
        DEBUG;
        return(-1);  
    }
    return(listener);
}

int main(int argc, char *argv[])
{
    int ret, epfd;
    thread_pool_t * pools;
    struct epoll_event ev, revents;

    proc_ignore_sigpipe();
    pools = create_pools(THREADNUM);
    if (!pools) {
        perror("create_pools!!\n");
        return 1;
    }

    ret = creat_threads(THREADNUM, pools);
    if (ret) {
        perror("creat_threads!!\n");
        return 1;
    }



    wait_all_threads_ready(pools, THREADNUM);

    epfd = epoll_create(3);
    if(epfd < 0){
        printf("[main] %s\n", strerror(errno));
        perror("epoll_create");
        return -1;
    }

    ev.events = EPOLLIN;
    ev.data.fd = listener;

    //add listener fd to epoll fd
    if(epoll_ctl(epfd, EPOLL_CTL_ADD, listener, &ev) < 0){
        perror("epoll_ctl");
        return 1;		
    }

    while(1){		
        ret = epoll_wait(epfd, &revents, 1, 1000);
        if (ret > 0)
        {
            if (revents.events & EPOLLIN)
            {
                client_fd = accept(listener, NULL, NULL);
                if(client_fd < 0){
                    //printf("client_fd < 0\n");
                    //break;
                    continue;
                }

                task_allocation(pools, THREADNUM);
            }
        }
    }

    return 0;
}

