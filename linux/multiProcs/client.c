#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <signal.h>
#include <poll.h>
#include <errno.h>
#include <sys/resource.h>

#include <netinet/tcp.h>

#define SERVERADDR "0.0.0.0"
#define CLIENTADDR "127.0.0.1"
#define SERVERPORT 8888
#define CLIENTPORT 9999

#define THREADNUM   1
#define MAX_RLIMIT_NOFILE 10000

pthread_attr_t attr;
#define THREAD_STACK_SIZE 20*1024

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
	char MACSTR[33];
	Status_t status;
}thread_pool_t;

void thr_sleep(uint32_t sec)
{
#ifdef _WIN32
  Sleep(sec*1000);
#else
  struct timeval t_val;

   t_val.tv_sec = sec;
   t_val.tv_usec = 0;

  select(0, NULL, NULL, NULL, &t_val);
#endif
}



static int proc_ignore_sigpipe(void)
{
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	sigemptyset(&act.sa_mask);
	act.sa_handler = SIG_IGN;
	act.sa_flags = 0;
	if(sigaction(SIGPIPE, &act, NULL) < 0){
		return 1;
	}
	return 0;
}

int SetSockFdAttr(int SockFd)
{
	int bReuseAddr = 1;
	struct linger OptLinger;

	OptLinger.l_onoff = 1;
	OptLinger.l_linger = 1;

	setsockopt(SockFd, SOL_SOCKET, SO_REUSEADDR, &bReuseAddr, sizeof(bReuseAddr));
	setsockopt(SockFd, SOL_SOCKET, SO_LINGER, &OptLinger, sizeof(OptLinger));
	setsockopt(SockFd, IPPROTO_TCP, TCP_NODELAY, &bReuseAddr, sizeof(bReuseAddr));
	//setsockopt (SockFd, SOL_TCP, TCP_CORK, &OptLinger, sizeof(OptLinger));


	return 0;
}



void *thread_all(void *arg)
{ 
	thread_pool_t *worker = (thread_pool_t *)arg;    


	pthread_mutex_lock(&worker->mutex);
	while (worker->status == NO)
		pthread_cond_wait(&worker->cond, &worker->mutex);
	worker->status = NO;
	//memcpy(pMACSTR, worker->MACSTR, strlen(worker->MACSTR));
	pthread_mutex_unlock(&worker->mutex);



	struct sockaddr_in  serverend, clientend;
	char buf[512], ipbuf[16];
	socklen_t len;
	int sd, ret;
	len = sizeof(serverend);

	serverend.sin_family = AF_INET;
	serverend.sin_port = htons(SERVERPORT);
	inet_pton(AF_INET, SERVERADDR, &serverend.sin_addr);

	clientend.sin_family = AF_INET;
	clientend.sin_port = htons(0);
	inet_pton(AF_INET, CLIENTADDR, &clientend.sin_addr);


	while (1)
	{
		sd = socket(AF_INET, SOCK_STREAM, 0);
		if (-1 == sd)
		{
			perror("socket");
			exit(1);
		}

		ret = bind(sd, (struct sockaddr *)&clientend, len);
		SetSockFdAttr(sd);


		if(connect(sd, (struct sockaddr*)&serverend, sizeof(serverend)) < 0)
		{
			printf("connect error:[errno %d : %s]\n", errno, strerror(errno));
			close(sd);
			continue;
		}
		break;
	}
#if 1
	while (1)
	{
		sprintf(buf, "hello world!\n", 30);

		if (write(sd, buf, 512) < 0)
		{
			printf("errno %d, %s\n", errno, strerror(errno));
			fprintf(stderr, "%s\n", strerror(errno));
		}
#if 0
		if (read(sd, buf, 512) < 0)
		{
			printf("%s\n", strerror(errno));
		}
#endif
		printf("%s\n", buf);
		
		thr_sleep(5);
	//	pause();
	}	
#endif 
	while (1)
		sleep(10000);

	return NULL;
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

static int task_allocation(thread_pool_t * pools, int thread_num)
{
	int i;


	for (i = 0; i < thread_num; i++) {
		pthread_mutex_lock(&pools[i].mutex);
		pools[i].status = OK;
		pthread_cond_signal(&pools[i].cond);
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


int main(void)
{
	int ret;
	thread_pool_t * pools;

	struct rlimit limit;
	limit.rlim_cur = MAX_RLIMIT_NOFILE;
	limit.rlim_max = MAX_RLIMIT_NOFILE;
	setrlimit(RLIMIT_NOFILE, &limit);
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

	task_allocation(pools, THREADNUM);

	while (1)
		sleep(100000);

	return 0;
}
