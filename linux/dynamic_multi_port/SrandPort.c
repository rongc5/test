#include "SrandPort.h"

pthread_mutex_t newsd_lock;
int connectnum = 0;
//char *ptr;
int generate_dynamic_RandPort(void)
{   

    time_t cur;
    int port;

    time(&cur);
    struct tm *res = localtime(&cur);
    struct tm tmp;
    memcpy(&tmp, res, sizeof(struct tm));
#if DEBUG
    tmp.tm_sec = 0;
#else
    tmp.tm_sec = 0;
    tmp.tm_hour = 0;
    tmp.tm_min = 0;
#endif

    cur = mktime(&tmp);

    srand(cur);
    do{
        port = (rand()%10000 + 10000);
    }while (port < 10000 || port > 20000);
#if DEBUG
    printf("generate dynamic port = %d\n", port);
#endif
    return port;
}

int Connect(int SockFd, char *addr, int domain, int port)
{
    struct sockaddr_in serverend;

    int len = sizeof(serverend);
    int tmp = 0, ret;

next:    serverend.sin_family = domain;
         serverend.sin_port = htons(port);
         inet_pton(domain, addr, &serverend.sin_addr);

         ret = connect(SockFd, (struct sockaddr *)&serverend, len);
         if (ret == -1) {
             perror("connect");
             port++;
             tmp++;
             if (tmp >= PORT_NUM)
             {
#if DEBUG
                 printf("链接次数超限!\n");
#endif
                 port = generate_dynamic_RandPort();	
             }
             goto next;
         }
         return 0;
}

int InitSockFd(int port, char *addr, int domain, int type, int protocol)
{
    struct sockaddr_in hisend;
    int len, SockFd, ret;

    len = sizeof(hisend);
    hisend.sin_family = domain;
    hisend.sin_port = htons(port);
    inet_pton(domain, addr, &hisend.sin_addr);

restart:    
    SockFd = socket(domain, type, protocol);
    if (-1 == SockFd)
    {
#if DEBUG
        printf("socket 创建失败!\n");
#endif
        perror("socket");
        exit(1);
    }

    SetSockFdAttr(SockFd);

    ret = bind(SockFd, (struct sockaddr *)&hisend, len);
    if (ret == -1)
    {
#if DEBUG 
        printf("bind 失败!\n");
#endif
        close(SockFd);
        goto restart;
    }

    return SockFd;
}



int InitEpoll(int SockFd)
{
    struct epoll_event fds[2];
    int epfd, ret;

    epfd = epoll_create(1);

    fds[0].data.fd = SockFd;
    fds[0].events = EPOLLIN;
    fds[1].data.fd = 0;
    fds[1].events = EPOLLIN;

    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, SockFd, &fds[0]);
    {;}

    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, 0, &fds[1]);
    {;}

    return epfd;
}

int Init_TimerValue(double *curtimeval, double *nexttimeval)
{
    struct tm *res, tmp_tm;

    time_t cur, next;

    time(&cur);
    res = localtime(&cur);

    memcpy(&tmp_tm, res, sizeof(struct tm));

    if (tmp_tm.tm_hour < 3)
    {
        tmp_tm.tm_hour = 3;
        tmp_tm.tm_min = 0;
        tmp_tm.tm_sec = 0;
        next = mktime(&tmp_tm);
        *curtimeval = difftime(next, cur);
    }
    else {
        tmp_tm.tm_hour = 23;
        tmp_tm.tm_min = 59;
        tmp_tm.tm_sec = 59;
        next = mktime(&tmp_tm) + 3*3600;
        *curtimeval = difftime(next, cur);
    }

    *nexttimeval = 24*3600;



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

    return 0;
}


void * thread_handler(void *arg)
{
    int newsd = (int)arg;
    struct pollfd fds;
    char buf[512];
    int ret;

    fds.fd = newsd;
    fds.events = POLLIN;

    ret = poll(&fds, 1, 1000); 
    if (0 != ret)
    {
LL:
        ret = read(newsd, buf, 512);
        if (-1 == ret)
        {	
            if (errno == EINTR)
            {
                goto LL;
            }
        }
        if (ret > 0)
        {
            buf[ret] = '\0';

            printf("From client: %s\n", buf);
            sprintf(buf, "server thread ID = %lu dealed the task!", pthread_self());
            write(newsd, buf, strlen(buf));
        }

    }
    printf("close newsd\n");
    close(newsd);

    pthread_mutex_lock(&newsd_lock);
    connectnum--;
    pthread_mutex_unlock(&newsd_lock);

    return NULL;
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

    ret = semctl(semid, 1, SETALL, array);
    if (ret == -1) {
        perror("semctl");
        return -1;
    }
    return semid;
}

void P(int semid, int numth)
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

void V(int semid, int numth)
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


void task(Prolib_t * ptr)
{   
    int sockfd, newsd, len, ret;	
    int serverport = ptr->port;
    char ipbuf[16];
    int tmpsd = 0;
    struct pollfd fds;

    struct sockaddr_in clientend;
    len = sizeof(clientend);
    sockfd = InitSockFd(serverport, SERVERADDR, AF_INET, SOCK_STREAM, 0);
    listen(sockfd, MAXLISTENNUM);

    fds.fd = sockfd;
    fds.events = POLLIN;

    pthread_t tid;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_mutex_init(&newsd_lock, NULL);	

#if DEBUG
    printf("i am pid = %u, i am going to be working!\n", getpid());
#endif	

    while (1)
    {	
        ret = poll(&fds, 1, 1000);
        if (ret != 0)
        { 	
            newsd = accept(sockfd, (struct sockaddr *)&clientend, &len);
            printf("newsd %d\n", newsd);
            sleep(3);
            if (-1 == newsd)
            {
                perror("accept");
                goto OUT1;
            }

#if DEBUG
            printf("Client addr:%s, port:%d connect success!\n",
                    (char *)inet_ntop(AF_INET, &clientend.sin_addr, ipbuf, 16),  ntohs(clientend.sin_port));
#endif	

            pthread_create(&tid, &attr, thread_handler, (void *)newsd);		
            pthread_mutex_lock(&newsd_lock);
            connectnum++;
            pthread_mutex_unlock(&newsd_lock);
            tmpsd = newsd;
        }

        P(ptr->semid, ptr->semid_index);
        if (ptr->port != serverport)
        {	
            V(ptr->semid, ptr->semid_index);
            while (1)
            {	
                pthread_mutex_lock(&newsd_lock);
#if DEBUG 
                printf("step2::I am going to bind new port = %d!, connectnum = %d\n", ptr->port, connectnum);
#endif
                if (connectnum == 0)
                {	 close(sockfd);
                    break;
                }
                pthread_mutex_unlock(&newsd_lock);
                usleep(1000000);
            }  
            pthread_mutex_unlock(&newsd_lock); 
#if DEBUG
            printf("I am going to do the next!\n");
#endif                    
             goto OUT1;
        }
        V(ptr->semid, ptr->semid_index);    
    }

OUT1:
    return;
}


Prolib_t * process_lib_init(int n, int semid)
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
        ptr[i].semid = semid;
        ptr[i].semid_index = i;
        ptr[i].port = 0;
    }

    return ptr;
}

void child_work(int numth, Prolib_t *lib)
{
    while (1)
    {	
    	 
        while (1)
        {	
        	P(lib[numth].semid, lib[numth].semid_index);
        	if (!(lib[numth].port))
        	{	
        		V(lib[numth].semid, lib[numth].semid_index);
            	sleep(1);            	
            }
            else 
            {
            	V(lib[numth].semid, lib[numth].semid_index);
            	break;
            }
        }
#if DEBUG
        printf("i am going to working!\n");
#endif
        task(&lib[numth]);
    }
}

int create_child_and_init_lib(int n, Prolib_t *lib, void(*child_func)(int numth, Prolib_t *lib))
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
            V(lib[i].semid, lib[i].semid_index);
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

Prolib_t * create_dynamic_process_libs(int n)
{
    Prolib_t * lib;
    int ret, semid;

    semid = sem_init(n);
    if (semid == -1)
        return NULL;

    lib = process_lib_init(n, semid);
    if (NULL == lib)
        return NULL;

    ret = create_child_process(n, lib);
    if (ret == -1)
        return NULL;

    return lib;
}

int assign_job(Prolib_t * lib, int numth, int serverport)
{  
    P(lib[numth].semid, lib[numth].semid_index);
    lib[numth].port = serverport;   
#if DEBUG
    printf("第 %d 个端口, 端口为 %d\n", numth, lib[numth].port);
#endif
    V(lib[numth].semid, lib[numth].semid_index);
    return 0;
}
