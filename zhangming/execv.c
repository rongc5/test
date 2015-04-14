#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{
    int ret;
    char * v[] = {
        "ls",
        "-al",
        "/",
        NULL //attiton!!!!
    };

    //ret = execv("/bin/ls", v);
    ret = execlp("cd","cd", "/", NULL);
    if (ret == -1) {
        perror("execl");
        exit(1);
    }

    printf("ok!\n");
    exit(0);
}
