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
#include <time.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdarg.h>
#include <sys/epoll.h>
#include <sys/time.h>
#include <sys/resource.h>


#define MAX_RLIMIT_NOFILE 100000*2
#define MAX_LINK    200000

#define SERVERADDR "0.0.0.0"
#define SERVERPORT 8888

int set_nonblock(int fd)
{
    int flags;
    flags = fcntl(fd, F_GETFL, 0);
    if (flags < 0)
    {
        //logError("file: "__FILE__", line: %d, fcntl fail, errno: %d, error info: %s.", __LINE__, errno, strerror(errno));
        return errno != 0 ? errno : EACCES;
    }
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
    {
        //logError("file: "__FILE__", line: %d, fcntl fail, errno: %d, error info: %s.", __LINE__, errno, strerror(errno));
        return errno != 0 ? errno : EACCES;
    }
    return 0;
}

int main(int c, char **v)
{

    int sd, ret, newsd, epfd, flags;;
#if 0
    struct rlimit limit;
    limit.rlim_cur = MAX_RLIMIT_NOFILE;
    limit.rlim_max = MAX_RLIMIT_NOFILE;
    ret = setrlimit(RLIMIT_NOFILE, &limit);
    if (ret == -1) {
        perror("setrlimit...");
        exit(1);
    }
#endif
    struct sockaddr_in serverend, clientend; 
    struct epoll_event ev, revents;       

    socklen_t len;
    char ipbuf[16], buf[256];
    len = sizeof(serverend);

    serverend.sin_family = AF_INET;
    serverend.sin_port = htons(SERVERPORT);
    inet_pton(AF_INET, SERVERADDR, &serverend.sin_addr);

    epfd = epoll_create(3);
    if(epfd < 0){
        printf("[main] %s\n", strerror(errno));
        perror("epoll_create");
        return -1;
    }



    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == sd){
        perror("socket");
        exit(1);
    }

    set_nonblock(sd);	
    ret = bind(sd, (struct sockaddr *)&serverend, len);
    if (-1 == ret){
        perror("bind");
        exit(1);
    }


    ret = listen(sd, MAX_LINK);

    ev.events = EPOLLIN;
    ev.data.fd = sd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, sd, &ev);

    flags = fcntl(sd, F_GETFL, 0);
    if (flags < 0)
    {
        return errno != 0 ? errno : EACCES;
    }

    if (flags & O_NONBLOCK)
    {
        printf("sd is O_NONBLOCK\n");
    } else  {
        printf("sd is BLOCK\n");
    }     			


    while (1){ 
        ret = epoll_wait(epfd, &revents, 1, 1000);
        if (ret > 0)
        {
            printf("epoll...\n");
            if (revents.events & EPOLLIN)
            {
                newsd = accept(sd, (struct sockaddr *)&clientend, &len);
                if (-1 == newsd){
                    perror("accept");
                    close(newsd);
                    continue;
                }

                flags = fcntl(newsd, F_GETFL, 0);
                if (flags < 0)
                {
                    return errno != 0 ? errno : EACCES;
                }

                if (flags & O_NONBLOCK)
                {
                    printf("newsd is O_NONBLOCK\n");
                } else {
                    printf("newsd is BLOCK\n");
                }     			



                printf("Client addr:%s, client port:%d connect success!, newsd:%d\n",(char *)inet_ntop(AF_INET, &clientend.sin_addr, ipbuf, 16),  ntohs(clientend.sin_port), newsd); 
#if 0
                memset(buf, 0, 512);
                ret = read(sd, buf, 512);
                buf[ret - 1] = '\0';
                printf("recvfrom client:%s\n", buf);
                write(newsd, "yes!", sizeof("yes!"));
#endif
            }
        }
        //close(newsd);
        //sleep(10);
    }
}

