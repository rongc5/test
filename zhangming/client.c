#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <errno.h>

#define KEYPATH "/etc/passwd"
#define KEYID 100
#define MAXSIZE 256
#define REQTYPE 1

typedef struct req_data
{
    int transType;
    char filePath[MAXSIZE];
}req_data_t;

typedef struct req_msg
{
    long mtype;
    req_data_t data;
}req_msg_t;

typedef struct rep_data
{
    int size;
    char content[MAXSIZE];
}rep_data_t;

typedef struct rep_msg
{
    long mtype;
    rep_data_t data;
}rep_msg_t;

int msg_init(void)
{
    int msgid;
    key_t key;

    key = ftok(KEYPATH, KEYID);
    if (-1 == key)
    {
        perror("ftok");
        return -1;
    }
    msgid = msgget(key, IPC_CREAT | 0600);
    {
        if (-1 == msgid)
        {
            perror("msgget");
            return -1;
        }
    }

    return msgid;
}

int send_server_req(int msgid, req_msg_t *req)
{
    int ret;
    int size;

    size = sizeof(*req) - sizeof(long);
    ret = msgsnd(msgid, req, size, 0);
    if (-1 == ret)
    {
        perror("msgrcv");
        exit(1);
    }
    return 0;
}

void recv_data(int msgid, char *dstpath)
{
    rep_msg_t msg;
    int fd, ret, size, type;

    size = sizeof(msg) - sizeof(long);
    type = getpid();

    fd = open(dstpath, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (-1 == fd)
    {
        perror("open");
        return;
    }
    while (1)
    {
        ret = msgrcv(msgid, &msg, size, type, 0);
        if (-1 == ret)
        {
            perror("msgrcv");
            break;
        }
        if (msg.data.size == 0)
            break;
        write(fd, msg.data.content, msg.data.size);
        usleep(10000);
        write(1, "...", 3);
    }
    return;
}

int main(int c, char **v)
{
    if (c != 3)
    {
        fprintf(stderr, "Usage...\n");
        exit(1);
    }

    int msgid;
    int ret;
    req_msg_t req;

    msgid = msg_init();
    if (-1 == msgid)
    {
        exit(1);
    }
    req.mtype = REQTYPE;
    req.data.transType = getpid();
    strncpy(req.data.filePath, v[1], MAXSIZE);

    ret = send_server_req(msgid, &req);
    if (-1 == ret)
     exit(1);
    recv_data(msgid, v[2]);

    exit(1);
}
