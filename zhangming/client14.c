#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <fcntl.h>
#include <signal.h>
#include <poll.h>
#include <sys/time.h>

#define SERVERADDR "192.168.1.103"
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
    struct sockaddr_in serverend;
    socklen_t len;
    int ret, sd, fd, errnum;
    char buf[512];
    int i = 0;

    struct itimerval cur;
    struct sigaction act;

    act.sa_handler = timer_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_RESTART;

    sigaction(SIGALARM, &act, NULL);

    cur.it_value.tv_sec = 0;
    cur.it_value.tv_usec = 300000;

}
