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
	int keepalive = 1; // 开启keepalive属性
	int keepidle = 10; // 如该连接在60秒内没有任何数据往来,则进行探测
	int keepinterval = 1; // 探测时发包的时间间隔为5 秒
	int keepcount = 3; // 探测尝试的次数.如果第1次探测包就收到响应了,则后2次的不再发.



	while (1)
	{
		newsd = accept(sd, (struct sockaddr *)&clientend, &len);
		if (-1 == newsd){
			perror("accept");
			exit(1);
		}

		printf("Client addr:%s, port:%d connect success!\n",
				(char *)inet_ntop(AF_INET, &clientend.sin_addr, ipbuf, 16),  ntohs(clientend.sin_port));
		
		ret = setsockopt(newsd, SOL_SOCKET, SO_KEEPALIVE, (void *)&keepalive , sizeof(keepalive ));
		if (ret == -1){
			printf("setsockopt:%s, %d\n", strerror(errno), __LINE__);
			return 1;
		}
		ret = setsockopt(newsd, SOL_TCP, TCP_KEEPIDLE, (void*)&keepidle , sizeof(keepidle ));
		if (ret == -1){
			printf("setsockopt:%s, %d\n", strerror(errno), __LINE__);
			return 1;
		}
		ret = setsockopt(newsd, SOL_TCP, TCP_KEEPINTVL, (void *)&keepinterval , sizeof(keepinterval ));
		if (ret == -1){
			printf("setsockopt:%s, %d\n", strerror(errno), __LINE__);
			return 1;
		}
		ret = setsockopt(newsd, SOL_TCP, TCP_KEEPCNT, (void *)&keepcount , sizeof(keepcount ));	
		if (ret == -1){
			printf("setsockopt:%s, %d\n", strerror(errno), __LINE__);
			return 1;
		}
		ret = read(newsd, buf, 30);
		if (0 == ret){
			perror("read:...");
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

