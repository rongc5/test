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

int main(void)
{
    int sd, ret;
    struct sockaddr_un serverend;
    socklen_t len;
    stu_t tmp;
    stu_t *buf;

    len = sizeof(serverend);
    serverend.sun_family = AF_UNIX;
    strncpy(serverend.sun_path, SERVERPATH, 108);

    sd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sd == -1) {
        perror("socket");
        exit(1);
    }

    unlink(SERVERPATH);
    ret = bind(sd, (struct sockaddr *)&serverend, len);
    if (ret == -1) {
        perror("bind");
        exit(1);
    }

    while (1)
    {
        ret = recvfrom(sd, &tmp, sizeof(tmp), MSG_PEEK, NULL, NULL);
        if (ret == -1) {//MSG_PEEK为查看命令，原数据还在
            perror("recvfrom");
            exit(1);
        }
        printf("packet size = %d\n", tmp.packetlen);
        buf = (stu_t *)malloc(tmp.packetlen);
        if (!buf) {
            perror("malloc");
            exit(1);
        }
        ret = recvfrom(sd, buf, tmp.packetlen, 0, NULL, NULL);
        if (ret == -1) {
            perror("recvfrom");
            exit(1);
        }
        printf("name=%s\n", buf->name);
        printf("chinese=%d\n", buf->chinese);
        printf("math=%d\n", buf->math);
    }
}
