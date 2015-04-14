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

#define SERVERADDR "192.168.1.104"
#define SERVERPORT 8888
static int token = 0;
#define CPS 30
#define BURST 5000

void timer_handler(int s)
{
    if ((token += CPS) >= BURST)
        token = 0;
}

int main(int c, char **v)
{
    if (c != 3)
    {
        fprintf(stderr, "usage...\n");
        exit(1);
    }

    struct sockaddr_in serverend;
    char buf[512];
    socklen_t len;
    int sd, ret, fd, errnum, i = 0;
    struct itimerval cur;
    struct sigaction act;

    act.sa_handler = timer_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_RESTART;

    sigaction(SIGALRM, &act, NULL);

    cur.it_value.tv_sec = 0;
    cur.it_value.tv_usec = 300000;
    cur.it_interval.tv_sec = 0;
    cur.it_interval.tv_usec = 300000;

    setitimer(ITIMER_REAL, &cur, NULL);

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

    ret = connect(sd, (struct sockaddr *)&serverend, len);
    if (-1 == ret)
    {
        perror("connect");
        exit(1);
    }
    printf("connect success!\n");
    sprintf(buf, "%s", v[1]);

    ret = write(sd, buf, strlen(buf));
    if (-1 == ret)
    {
        perror("write");
        exit(1);
    }

    fd = creat(v[2], 0666);
    if (-1 == fd)
    {
        perror("creat");
        exit(1);
    }

    while (1)
    {
        while (0 == token)
            pause();

        ret = read(sd, buf, CPS);
        if (0 == ret)
            break;

        if ((i == 0) && (strncmp(buf, "errno", 5) == 0))
        {
        read(sd, &errnum, 4);
        fprintf(stderr, "File:%s\n", strerror(errnum));
        exit(1);
        }
        i++;
        token -= CPS;
        printf("......");
        fflush(stdout);
        write(fd, buf, ret);
    }
    close(fd);
    close(sd);
    exit(0);
}

