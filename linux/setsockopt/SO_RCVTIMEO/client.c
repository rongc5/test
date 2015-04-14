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
#include <sys/time.h>

#define SERVERADDR "0.0.0.0"
#define LOCALADDR "127.0.0.1"
#define SERVERPORT 8888


int main(int c, char **v)
{

	struct sockaddr_in serverend, clientend;
	char buf[512], tmp[512];
	socklen_t len;
	int sd, ret, fd, errnum, i = 0;

    scanf("%s", tmp);
    len = sizeof(serverend);

	serverend.sin_family = AF_INET;
	serverend.sin_port = htons(SERVERPORT);
	inet_pton(AF_INET, SERVERADDR, &serverend.sin_addr);

	clientend.sin_family = AF_INET;
	clientend.sin_port = htons(0);
	inet_pton(AF_INET, LOCALADDR, &clientend.sin_addr);

	sd = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == sd){
		perror("socket");
		exit(1);
	}

	ret = bind(sd, (struct sockaddr *)&clientend, len);

	ret = connect(sd, (struct sockaddr *)&serverend, len);
	if (-1 == ret){
		perror("connect");
		exit(1);
	}
	printf("connect success!\n");
    while (1){
        printf("tmp:%s\n", tmp);
        ret = write(sd, tmp, strlen(buf));
        if (-1 == ret){
            perror("write");
            exit(1);
        }
        for (i = 0; i < 6; i++){
          ret = read(sd, buf, sizeof(buf));
          if (ret < 0){
            perror("read");
            return -1;
          }
          buf[ret -1] = '\0';
          printf("...%s\n", buf);
        }
        //sleep(11);
    }

	exit(0);
}

