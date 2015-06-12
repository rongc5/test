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

#include <sys/shm.h>
#include <signal.h>


#define SERVERADDR "0.0.0.0"
#define SERVERPORT 8888

#define TYPE_FIND_NUM 0
#define TYPE_FIND_INDEX 1


typedef void (*task_t)(void *);

typedef enum
{
    UNPROCEED,
    IDLE,
    RUNNING
}pro_status;

typedef struct prolib
{
    pid_t pid;
    int semid;
    int semid_index;
    task_t func;
    int sokFd;
    pro_status status;
    int balance;
}Prolib_t;


void task(void *arg)
{
    struct sockaddr_in clientend;        
    int sd = *(int *)arg;
    int newsd, ret;
    char buf[512], ipbuf[16];
    int len = sizeof(struct sockaddr_in);
    newsd = accept(sd, (struct sockaddr *)&clientend, &len);
    if (-1 == newsd)
    {
        perror("accept");
        return ;
    }
    printf("Client addr:%s, port:%d connect success!\n",
            (char *)inet_ntop(AF_INET, &clientend.sin_addr, ipbuf, 16),  ntohs(clientend.sin_port));

    ret = fcntl(newsd, F_GETFL);
    ret = fcntl(newsd, F_SETFL, ret | O_NONBLOCK);
    while (1)
    {
        ret = read(newsd, buf, 512);
        if (-1 == ret)
        {
            perror("read");
           // return;
        }
        else if (ret == 0)
        {
            close(newsd);
            //return;
        }
        buf[ret] = '\0';
        printf("I am pid = %d, recv from client:%s\n", getpid(), buf);
        //sprintf(buf, "I recved you msg!, I am pid = %d\n", getpid(), 50);
      //  write(newsd, buf, 50);
    }
}

int InitEpoll(int SockFd)
{
    struct epoll_event fds;
    int epfd, ret;

    epfd = epoll_create(1);

    fds.data.fd = SockFd;
    fds.events = EPOLLIN | EPOLLET;

    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, SockFd, &fds);
    {;}

    return epfd;
}


int sem_init(int n)
{
    key_t key;
    int ret;
    unsigned short array[n];
    int semid;

    memset(array, 0, sizeof(short)*n);
    key = IPC_PRIVATE;
    semid = semget(key, n, IPC_CREAT | 0600);
    if (-1 == semid)
    {
        perror("semget");
        return -1;

    }

    ret = semctl(semid, 0, SETALL, array);
    if (ret == -1) {
        perror("semctl");
        return -1;
    }
    return semid;
}

void p(int semid, int numth)
{
    struct sembuf op;
    int ret;

    op.sem_num = numth;
    op.sem_op = -1;
    op.sem_flg =0;

    while (1)
    {
        ret = semop(semid, &op, 1);
        if (ret == -1)
        {
            perror("semop");
            continue;
        }else
            break;
    }
}

void v(int semid, int numth)
{
    struct sembuf op;
    int ret;

    op.sem_num = numth;
    op.sem_op = 1;
    op.sem_flg = 0;
    while (1)
    {
        ret = semop(semid, &op, 1);
        if (-1 == ret)
        {
            perror("semop");
            continue;
        }else
            break;
    }
}

Prolib_t * process_lib_init(int n, int semid, int sokFd)
{
    Prolib_t * ptr;
    int i, size;

    size = sizeof(Prolib_t) * n;

    ptr = (Prolib_t *)mmap(NULL, size, PROT_READ | PROT_WRITE, 
            MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (ptr == MAP_FAILED)
    {
        perror("mmap");
        return NULL;
    }

    for (i = 0; i < n; i++)
    {
        ptr[i].status = UNPROCEED;
        ptr[i].balance = 0;
        ptr[i].semid = semid;
        ptr[i].semid_index = i;
        ptr[i].sokFd =sokFd;
        ptr[i].func = NULL;
    }

    return ptr;
}

static void increase_job_times(Prolib_t *worker)
{
    worker->balance++;
}

static void set_process_status(int numth, Prolib_t *lib, pro_status status)
{
    lib[numth].status = status;
}



static void child_work(int numth, Prolib_t *lib)
{
    while (1)
    {
        set_process_status(numth, lib, IDLE);
        p(lib[numth].semid, lib[numth].semid_index);
        set_process_status(numth, lib, RUNNING);
        lib[numth].func((void *)&lib[numth].sokFd);
        increase_job_times(lib + numth);
    }
}

static int create_child_and_init_lib(int n, Prolib_t *lib, void(*child_func)(int numth, Prolib_t *lib))
{
    int i;
    pid_t pid;
    for (i = 0; i < n; i++)
    {
        pid = fork();
        if (-1 == pid )
        {
            perror("fork");
            return -1;
        }
        if (0 == pid)
        {
            lib[i].pid = getpid();
            child_func(i, lib);
            exit(0);
        }
    }

    return 0;
}

int create_child_process(int n, Prolib_t *lib)
{
    int ret;
    ret = create_child_and_init_lib(n, lib, child_work);
    return ret;
}

static void wait_child_ready(Prolib_t *lib, size_t n)
{
    int i;

    for (i = 0; i < n; i++)
    {
        if (lib[i].status == UNPROCEED)
            i--;
    }
}

Prolib_t * create_dynamic_process_libs(int sokFd, int n)
{
    int semid;
    Prolib_t * lib;
    int ret;

    semid = sem_init(n);
    if (semid == -1)
        return NULL;

    lib = process_lib_init(n, semid, sokFd);
    if (NULL == lib)
        return NULL;

    ret = create_child_process(n, lib);
    if (ret == -1)
        return NULL;

    wait_child_ready(lib, n);
    return lib;
}

static int find_idle_worker(Prolib_t *lib, size_t size, int type)
{
    int ret, i;
    int index = -1, cnt = -1, num = 0;
    for (i = 0; i < size; i++)
    {
        if ((lib[i].status == IDLE))
        {
	    num++;
            if (-1 == cnt)
            {
                cnt = lib[i].balance;
                index = i;
            }
            else
            {
                if (cnt > lib[i].balance)
                {
                    cnt = lib[i].balance;
                    index = i;
                }
            }
        }
    }
	if (type == TYPE_FIND_INDEX)
    	return index;
	else if (type == TYPE_FIND_NUM)
	return num;
}


static void notify_worker(Prolib_t * lib, int numth)
{
    v(lib[numth].semid, lib[numth].semid_index);
}

static int init_shmid(int sockFd)
{
    char *ptr;
    int shmid = shmget(IPC_PRIVATE, 4, IPC_CREAT | 0600);
        if (-1 == shmid)
        {
            perror("shmget");
            return -1;
        }

    ptr = (char *)shmat(shmid, NULL, 0);
    if (ptr == (void *)-1)
    {
        perror("shmat");
        return -1;
    }

    *ptr = sockFd;

    return shmid;
}

void desory_zmobe(void)
{
    struct sigaction act;
    
    act.sa_handler = SIG_IGN;
    act.sa_flags = SA_NOCLDWAIT;
    sigemptyset(&act.sa_mask);

    sigaction(SIGCLD, &act, NULL);
}

static void new_worker(Prolib_t *lib, size_t size, int shmid)
{
    char * ptr;
    pid_t pid;
    int ret;
    ptr = shmat(shmid, NULL, 0);
    if (ptr == (void *)-1)
    {
        perror("shmat");
        return ;
    }

    pid = fork();
    if (pid == 0)
    {
        while (1)
        {
            task((void *)ptr);
            ret = find_idle_worker(lib, size, TYPE_FIND_NUM);
            if (ret >= size/3 )
              {
		shmdt(ptr);
                break;
	      }
        }
        exit (0);
    }    
}

static void append_job_to_worker(Prolib_t *lib, int numth, task_t task)
{
    lib[numth].func = task;
}

int assign_job(Prolib_t * lib, size_t libsize, task_t task)
{
    int numth;
    numth = find_idle_worker(lib, libsize, TYPE_FIND_INDEX);
    if (-1 == numth)
    {
        fprintf(stderr, "All process are busy!\n");
        return -1;
    }
    append_job_to_worker(lib, numth, task);
    notify_worker(lib, numth);

    return 0;
}


int main(void)
{
    struct sockaddr_in serverend;

    socklen_t len;
    int sd, ret, shmid, epfd;
    struct epoll_event revents;
    len = sizeof(serverend);
    pid_t pid;

    serverend.sin_family = AF_INET;
    serverend.sin_port = htons(SERVERPORT);
    inet_pton(AF_INET, SERVERADDR, &serverend.sin_addr);

    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == sd)
    {
        perror("socket");
        exit(1);
    }

    ret = bind(sd, (struct sockaddr *)&serverend, len);
    if (-1 == ret)
    {
        perror("bind");
        exit(1);
    }
    
    ret = listen(sd, 30);

    ret = fcntl(sd, F_GETFL);
    ret = fcntl(sd, F_SETFL, ret | O_NONBLOCK);

    if (-1 == ret)
	printf("listen\n");

    desory_zmobe();
   
    Prolib_t *lib = create_dynamic_process_libs(sd, 3);
    epfd = InitEpoll(sd);

    shmid = init_shmid(sd);
    while (1)
    {

        ret = epoll_wait(epfd, &revents, 1, -1);
        if (ret >= 0)
        {
            if (revents.events & EPOLLIN)
            {
                printf("new connect is coming!, ret: %d\n", ret);
                ret = assign_job(lib, 3, task);
                if (-1 == ret)
                    new_worker(lib, 3, shmid);
            }
        }
    }
}





