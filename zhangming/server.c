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

int get_client_req(int msgid, req_msg_t *req)
{
    int ret;
    int size;

    size = sizeof(*req) - sizeof(long);
    ret = msgrcv(msgid, req, size, REQTYPE, 0);
    if (-1 == ret)
    {
        perror("msgrcv");
        exit(1);
    }
    return 0;
}

static void send_file_data(int msgid, char *fp, int type)
{
    int fd, ret, size;
    rep_msg_t rep;
    rep.mtype = type;
    size = sizeof(rep) - sizeof(long);

    fd = open(fp, O_RDONLY);
    if (-1 == fd)
    {
        perror("open");
        exit(1);
    }

    while (1)
    {
        ret = read(fd, rep.data.content, MAXSIZE);
        rep.data.size = ret;
        msgsnd(msgid, &rep, size, 0);
        if (0 == ret)
            break;
    }
}

int process_client_req(int msgid, req_msg_t *req)
{
    int type;
    char *fp;
    pid_t pid;

    type = req->data.transType;
    fp = req->data.filePath;

    pid = fork();
    if (-1 == pid)
    {
        perror("fork");
        return -1;
    }
    if (pid > 0)
        return 0;
    send_file_data(msgid, fp, type);
    exit(0);
}

void destory_zombie(void)
{
    struct sigaction act;

    act.sa_handler = SIG_IGN;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_NOCLDWAIT;

    sigaction(SIGCHLD, &act, NULL);
}

int main(void)
{
    int msgid;
    int ret;
    req_msg_t req;
   
    destory_zombie();
    msgid = msg_init();
    if (-1 == msgid)
    {
        exit(1);
    }

    while (1)
    {
        ret = get_client_req(msgid, &req);
        if (-1 == ret)
        break;
        ret = process_client_req(msgid, &req);
    }
}
