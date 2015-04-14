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

    strcpy(a.name, "张");
    a.math = 99;
    a.chinese = 100;
    data.mtype = 1;
    data.info = a;
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


    ret = msgsnd(msgid, &data, msgsize, 0);
    if (-1 == ret)
    {
        perror("msgsnd");
        exit(1);
    }

    return 0;
}
