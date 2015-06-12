#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <string.h>

#define KEYPATH "/etc/passwd"
#define KEYID 88

struct stu
{
    char name[20];
    int math;
    int chinese;
};

struct msgbuf
{
    long mtype;
    struct stu info;
};

int main(void)
{
    key_t key;
    int msgid, ret;
    struct stu a;
    struct msgbuf data;
    int msgsize;
    struct msqid_ds buff;

    msgsize = sizeof(data) - sizeof(long);

    key = ftok(KEYPATH, KEYID);
    if (key == -1)
    {
        perror("ftok");
        exit(1);
    }

    msgid = msgget(key, IPC_CREAT | 0600);
    if (msgid == -1)
    {
        perror("msgget");
        exit(1);
    }

    ret = msgctl(msgid, IPC_STAT, &buff);
    if (-1 == ret)
    {
        perror("msgctl");
        exit(1);
    }
    if (buff.__msg_cbytes == 0)
    {
        ret = msgctl(msgid, IPC_RMID, NULL);
        if (-1 == ret)
        {
            perror("msgctl");
            exit(1);
        }
        exit(0);
    }

    ret = msgrcv(msgid, &data, msgsize, 1, MSG_NOERROR);
    if (-1 == ret)
    {
        perror("msgrcv");
        exit(1);
    }

    printf("name = %s\n", data.info.name);
    printf("math = %d\n", data.info.math);
    printf("chinese = %d\n", data.info.chinese);

    return 0;
}
