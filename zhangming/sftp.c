#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define KEYPATH  "/etc/passwd"
#define KEYID 88

struct stu{
    char name[10];
    char pwd[20];
};

struct msgbuf
{
    long mtype;
    struct stu info;
};

int main(void)
{
    pid_t pid;

    key_t key;
    int msgid, ret;
    struct stu a;
    struct msgbuf data;
    int msgsize;

    pid = fork();
    {}
    if (0 == pid)
    {
        printf("输入文件名:");
        scanf("%s", a.name);
        printf("\n输入路径名:");
        scanf("%s", a.pwd);

        data.mtype = 1;
        data.info = a;
        msgsize = sizeof(data) - sizeof(long);

        key = ftok(KEYPATH, KEYID);
        {}

        msgid = msgget(key, IPC_CREAT | 0600);
        {}

        ret = msgsnd(msgid, &data, msgsize, 0);
        {}

        exit(0);
    }
    else
    {}

    return 0;

}

