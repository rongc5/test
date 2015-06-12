#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef struct stu {
    int packetlen;
    int math;
    int chinese;
    char name[1];
} stu_t;

#define SERVERPATH "/tmp/serverend"

int main(int c, char **v)
{
    if (c != 2) {
        fprintf(stderr, "Usage...\n");
        exit(1);
    }

    int sd, ret;
    struct sockaddr_un serverend;
    socklen_t len;
    stu_t *tmp;
    size_t datalen = 0;
    len = sizeof(serverend);
    serverend.sun_family = AF_UNIX;
    strncpy(serverend.sun_path, SERVERPATH, 108);

    sd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sd == -1) {
        perror("socket");
        exit(1);
    }

    datalen = sizeof(stu_t) + strlen(v[1]);
    tmp = (stu_t *)malloc(datalen);
    if (!tmp) {
        perror("malloc");
        exit(1);
    }

    strcpy(tmp->name, v[1]);
    tmp->chinese = 99;
    tmp->math = 98;
    tmp->packetlen = datalen;
    ret = sendto(sd, tmp, datalen, 0, (struct sockaddr *)&serverend, len);
    if (ret == -1) {
        perror("sendto");
        exit(1);
    }

    close(sd);

    exit(0);
}
