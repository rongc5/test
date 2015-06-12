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

#include <netinet/tcp.h>



#define SERVERADDR "0.0.0.0"
#define SERVERPORT 8888

#define MAX_RECV_BUFF	1024*120
#define MAX_SEND_BUFF 1024*120


static int getTCP_NODELAY(int fd){
    int nodelay;
    socklen_t len;

    nodelay = 1;
    len = sizeof(nodelay);

    if (getsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &nodelay, &len) < 0){
        perror("getsockopt");
        return -1;
    }
	printf("func:%s	 TCP_NODELAY    nodelay is:%d\n", __func__, nodelay);
	return 0;
}


static int setTCP_NODELAY(int fd){
    int nodelay;
    socklen_t len;

    nodelay = 1;
    len = sizeof(nodelay);

    if (setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &nodelay, len) < 0){
        perror("setsockopt");
        return -1;
    }
	printf("func:%s	 TCP_NODELAY    nodelay is:%d\n", __func__, nodelay);
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
	getTCP_NODELAY(sd);
	setTCP_NODELAY(sd);
	getTCP_NODELAY(sd);
	ret = bind(sd, (struct sockaddr *)&serverend, len);
	if (-1 == ret)
	{
		perror("bind");
		exit(1);
	}

	listen(sd, 10);
	while (1)
	{
		newsd = accept(sd, (struct sockaddr *)&clientend, &len);
		if (-1 == newsd){
			perror("accept");
			exit(1);
		}

		printf("Client addr:%s, port:%d connect success!\n",
				(char *)inet_ntop(AF_INET, &clientend.sin_addr, ipbuf, 16),  ntohs(clientend.sin_port));
		getTCP_NODELAY(newsd);
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

