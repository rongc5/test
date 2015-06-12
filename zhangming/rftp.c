#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <sys/stat.h>

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

struct stu1
{
    int type;
    char buf1[256];
};

struct msgbuf1
{
    long mtype;
    struct stu1 info;
};

int main(void)
{
    pid_t pid;

    key_t key;
    int msgid, ret;
    struct stu a;
    struct msgbuf data;
    int msgsize;
    struct msqid_ds buff;
    int fd;
    int msgsize1;
    int ret1;

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
    pid = fork();
    if (0 == pid)
    {
        struct stu1 b;
        struct msgbuf1 data1;
        ret = chdir(data.info.pwd);
        {}
        fd = open(data.info.name, O_RDONLY);
        {}
        msgsize1 =sizeof(data1) - sizeof(long);
        while (1)
        {
            ret = read(fd, data1.info.buf1, 256);
            if (0 == ret)
                break;
         //   data1.info.buf1 = ret;
            ret1 = msgsnd(msgid, &data1, msgsize1, 0);
        }
        exit(0);
    }

    return 0;
}

