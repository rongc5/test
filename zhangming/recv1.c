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
    {}

    msgid = msgget(key, IPC_CREAT | 0600);
    {}

    ret = msgctl(msgid, IPC_STAT, &buff);
    {}

    if (buff.msg_qbytes == 0)
    {
        ret = msgctl(msgid, IPC_RMID, NULL);
        {}

        exit(0);
    }

    ret = msgrcv(msgid, &data, msgsize, 1, MSG_NOERROR);
    {}

    printf("name = %s\n", data.info.name);
    printf("math = %d\n", data.info.math);
    printf("chinese = %d\n", data.info.chinese);

    return 0;
}

