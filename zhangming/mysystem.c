#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int mysystem(char * cmd)
{
    pid_t pid;
    int ret;

    if (!cmd)
    {
        fprintf(stderr, "unkown command!\n");
        return -1;
    }

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return -1;
    }
    if (pid == 0)
    {
        execl("/bin/sh", "sh", "-c", cmd, NULL);
        exit(1);
    }
    wait(pid, NULL, 0);
    return 0;
}

int main(void)
{
    char buf[512];
    int ret;
    while (1)
    {
        printf("%%(qianfeng)>");
        fflush(stdout);
        ret = read(0, buf, 512);
        if (buf[ret - 1] == '\n')
            buf[ret - 1] = '\0';

        mysystem(buf);
    }
}
