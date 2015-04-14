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

static int getsockbuff(int fd){
	
	int send_buff = 0;
	int recv_buff = 0;
	int len = sizeof(send_buff);
	if(getsockopt(fd, SOL_SOCKET, SO_SNDBUF, (void *)&send_buff, &len ) < 0 ){
		perror("getsockopt: ");
		return -1;
	}
	

	if(getsockopt(fd, SOL_SOCKET, SO_RCVBUF, (void *)&recv_buff, &len ) < 0 ){
		perror("getsockopt: ");
		return -1;
	}

	printf("func:%s	 SO_SNDBUF is:%d,	SO_RCVBUF is:%d\n", __func__, send_buff, recv_buff);
	return 0;
}

static int setsockbuff(int fd){
	int send_buff = MAX_SEND_BUFF;
	int recv_buff = MAX_RECV_BUFF;
	int len = sizeof(send_buff);
	if(setsockopt(fd, SOL_SOCKET, SO_SNDBUF, (void *)&send_buff, len ) < 0 ){
		perror("getsockopt: ");
		return -1;
	}

	if(setsockopt(fd, SOL_SOCKET, SO_RCVBUF, (void *)&recv_buff, len ) < 0 ){
		perror("getsockopt: ");
		return -1;
	}
	
	
	printf("func:%s	 SO_SNDBUF is:%d,	SO_RCVBUF is:%d\n", __func__, send_buff, recv_buff);
	return 0;
}

static int setlinger(int fd){
	struct linger optlinger;

	optlinger.l_onoff = 1;  
	optlinger.l_linger = 5;  
	if (setsockopt(fd, SOL_SOCKET, SO_LINGER, &optlinger, sizeof(struct linger)) < 0){
		perror("setsockopt:");
		return -1;
	}	
	
	printf("func:%s	 SO_LINGER 	optlinger.l_onoff is:%d, optlinger.l_linger is:%d\n", __func__, optlinger.l_onoff, optlinger.l_linger);
	return 0;
}

static int getlinger(int fd){
	struct linger optlinger;

	optlinger.l_onoff = 0;  
	optlinger.l_linger = 0;  
	int len = sizeof(struct linger);
	if (getsockopt(fd, SOL_SOCKET, SO_LINGER, &optlinger, &len) < 0){
		perror("setsockopt:");
		return -1;
	}	
	
	printf("func:%s	 SO_LINGER 	optlinger.l_onoff is:%d, optlinger.l_linger is:%d\n", __func__, optlinger.l_onoff, optlinger.l_linger);
	return 0;
}









int main(void)
{
	struct sockaddr_in serverend, clientend;
	char buf[512], ipbuf[16];
	socklen_t len;
	int sd, ret, fd, newsd;
	len = sizeof(serverend);

	serverend.sin_family = AF_INET;
	serverend.sin_port = htons(SERVERPORT);
	inet_pton(AF_INET, SERVERADDR, &serverend.sin_addr);

	sd = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == sd)
	{
		perror("socket");
		exit(1);
	}
	getlinger(sd);
	setlinger(sd);
	//setsockbuff(sd);
	getlinger(sd);
	ret = bind(sd, (struct sockaddr *)&serverend, len);
	if (-1 == ret)
	{
		perror("bind");
		exit(1);
	}

	listen(sd, 10);
#if 0
	int setsockopt(int sockfd, int level, int optname,
                      const void *optval, socklen_t optlen);
#endif
	 struct timeval timeout;
    	timeout.tv_sec=1; // 2 second
    	timeout.tv_usec=0;
	while (1)
	{
		newsd = accept(sd, (struct sockaddr *)&clientend, &len);
		if (-1 == newsd){
			perror("accept");
			exit(1);
		}

		printf("Client addr:%s, port:%d connect success!\n",
				(char *)inet_ntop(AF_INET, &clientend.sin_addr, ipbuf, 16),  ntohs(clientend.sin_port));
		getlinger(newsd);	
		//setsockopt(newsd , SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(struct timeval));
		ret = read(newsd, buf, 30);
		if (0 == ret){
			close(newsd);
			continue;
		}else if (ret < 0){
			perror("read:...");
			close(newsd);
			continue;
		}
		buf[ret] = '\0';
		printf("buf: %s\n", buf);
			
		close(newsd);
	}
	return 0;
}

