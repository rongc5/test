#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <errno.h>
#include <sys/resource.h>

#include <netinet/tcp.h>

#define SERVERADDR "0.0.0.0"
#define CLIENTADDR "127.0.0.1"
#define SERVERPORT 8888
#define CLIENTPORT 9999

#define MAX_RLIMIT_NOFILE 100000*2

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


void thread_all(void *arg)
{ 
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


    while (1) {
        sd = socket(AF_INET, SOCK_STREAM, 0);
        if (-1 == sd){
            perror("socket");
            exit(1);
        }

        ret = bind(sd, (struct sockaddr *)&clientend, len);
        SetSockFdAttr(sd);

        if(connect(sd, (struct sockaddr*)&serverend, sizeof(serverend)) < 0){
            printf("connect error:[errno %d : %s]\n", errno, strerror(errno));
            close(sd);
            continue;
        }
        break;
    }

    printf("connect success! sd: %d\n", sd);

    while (1){
#if 0
        write(sd, "hello", sizeof("hello"));
        memset(buf, 0, 512);
        ret = read(sd, buf, 512);
        buf[ret - 1] = '\0';
        printf("recv from server:%s\n", buf);
#endif
        sleep(1);
    }

	return ;
}




int main(void)
{
	int ret;

        thread_all(NULL);

    return 0;
}

