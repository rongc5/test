#ifndef __SRAND__PORT_H
#define __SRAND__PORT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/time.h>
#include <string.h>
#include <sys/epoll.h>
#include <signal.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <pthread.h>
#include <poll.h>
#include <sys/select.h>



#define LOCALIPADDR1 "127.0.0.1"
#define LOCALIPADDR2 "127.0.0.2"
#define LOCALIPADDR3 "127.0.0.3"
#define SERVERADDR "0.0.0.0"


#define PORT_NUM 30

typedef struct prolib
{	
	pid_t pid;
    int semid;
    int semid_index;
    int port;
}Prolib_t;



int generate_dynamic_RandPort(void);
int InitSockFd(int port, char *addr, int domain, int type, int protocol);
int InitEpoll(int SockFd);

int SetSockFdAttr(int SockFd);
int Connect(int SockFd, char *addr, int domain, int port);

char * init_mmap_memory(void);

int SetSockFdAttr(int SockFd);

Prolib_t * process_lib_init(int n, int semid);

static void child_work(int numth, Prolib_t *lib);

int create_child_and_init_lib(int n, Prolib_t *lib, void(*child_func)(int numth, Prolib_t *lib));

int create_child_process(int n, Prolib_t *lib);

Prolib_t * create_dynamic_process_libs(int n);

int assign_job(Prolib_t * lib, int numth, int serverport);

void task(Prolib_t * ptr);

void * thread_handler(void *arg);

Prolib_t * create_dynamic_process_libs(int n);

int Init_TimerValue(double *curtimeval, double *nexttimeval);

void V(int semid, int numth);

void P(int semid, int numth);

int sem_init(int n);










#define MAXLISTENNUM 10





















#endif 
