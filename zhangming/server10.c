#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <string.h>

#define SERVERPATH "/tmp/serverend"

void proc_lib_work(int sd)
{
    int newsd;
    char buf[512];
    int ret;

    while (1)
    {
        newsd = accept(sd, NULL, NULL);
        if (newsd == -1) {
            perror("accept");
            exit(1);
        }

        ret = read(newsd, buf, 512);
        buf[ret] = '\0';
        printf("From client:%s\n", buf);
        write(newsd, "I am jack", 9);
        close(newsd);
    }
}

int main(void)
{
    int sd, ret, newsd;
    struct sockaddr_un serverend;
    socklen_t len;
    char buf[512];
    pid_t pid;
    int i;

    len = sizeof(serverend);
    serverend.sun_family = AF_UNIX;
    strcpy(serverend.sun_path, SERVERPATH);

    daemon(1, 1);
    sd = socket(AF_UNIX, SOCK_STREAM, 0);
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

    listen(sd, 10);

    for (i = 0; i < 10; i++)
    {
        pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(1);
        }
        if (0 == pid)
        {
            proc_lib_work(sd);
            exit(0);
        }
    }

    close(sd);
    exit(0);
}
