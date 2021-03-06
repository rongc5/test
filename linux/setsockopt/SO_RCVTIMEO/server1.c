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

#define SERVERADDR "0.0.0.0"
#define SERVERPORT 8888

#define MAX_RECV_BUFF	1024*120
#define MAX_SEND_BUFF 1024*120



static int setrcvtimo(int fd){
    struct timeval timeout;
    timeout.tv_sec=10;
    timeout.tv_usec=0;
    if (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(struct timeval)) < 0){
        perror("setsockopt:");
        return -1;
    }	

    printf("func:%s	 SO_RCVTIMEO 	timeout.tv_sec is:%d, timeout.tv_usec is:%d\n", __func__, timeout.tv_sec, timeout.tv_usec);
    return 0;
}


static int getrcvtimo(int fd){
    struct timeval timeout;
    timeout.tv_sec=0;
    timeout.tv_usec=0;
    int len = sizeof(struct timeval);
    if (getsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, &len) < 0){
        perror("setsockopt:");
        return -1;
    }	

    printf("func:%s	 SO_RCVTIMEO 	timeout.tv_sec is:%d, timeout.tv_usec is:%d\n", __func__, timeout.tv_sec, timeout.tv_usec);
    return 0;
}




int main(void)
{
    struct sockaddr_in serverend, clientend;
    char buf[512], ipbuf[16];
    socklen_t len;
    int sd, ret, fd, newsd;
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

   // getrcvtimo(sd);
    //setrcvtimo(sd);
   // getrcvtimo(sd);
    ret = bind(sd, (struct sockaddr *)&serverend, len);
    if (-1 == ret){
        perror("bind");
        exit(1);
    }

    listen(sd, 10);
//    while (1)
  //  {
        newsd = accept(sd, (struct sockaddr *)&clientend, &len);
        if (-1 == newsd){
            perror("accept");
            exit(1);
        }

        printf("Client addr:%s, port:%d connect success!\n",
                (char *)inet_ntop(AF_INET, &clientend.sin_addr, ipbuf, 16),  ntohs(clientend.sin_port));
        pid = fork();
        if (!pid){
            close(sd);
            setrcvtimo(newsd);
            while (1){
                getrcvtimo(newsd);
                ret = recv(newsd, buf, 512, 0);
                if (0 == ret){
                    if (errno == EINTR)
                        continue;
                    else if (errno == EAGAIN){
                        perror("read");
                        continue;
                        close(newsd);
                        exit(0);        
                    }
                    else {
                        break;
                    }
                }else if (ret < 0){
                    perror("read:...");
                    close(newsd);
                    exit(0);
                }
                buf[ret] = '\0';
                printf("pid:%u, buf: %s\n", getpid(), buf);
            }
        }
        else
        {	
        	printf(",...\n");
        	sleep(1000);
        }
        close(newsd);
        close(sd);
   // }
    return 0;
}

