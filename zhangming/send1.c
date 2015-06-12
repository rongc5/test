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

    strcpy(a.name, "allan");
    a.math = 99;
    a.chinese = 100;

    data.mtype = 1;
    data.info = a;
    msgsize = sizeof(data) - sizeof(long);

    key = ftok(KEYPATH, KEYID);
    {}

    msgid = msgget(key, IPC_CREAT | 0600);
    {}

    ret = msgsnd(msgid, &data, msgsize, 0);
    {}

    return 0;

}

