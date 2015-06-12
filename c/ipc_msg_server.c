#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define CPATH "/etc/passwd"
#define SPATH "/etc/passwd"
#define ID 88
#define SID 99

struct mymsg{
    long mtype;
    char mydate[256];
};


int main(int c, char **v)
{
    struct msqid_ds msg_buf;
    struct mymsg msg;
    char buf[256];
    int ret;
    int len;
    memset(&buf, 0, sizeof(struct msqid_ds));
#if 0
    key_t key_c = ftok(CPATH, ID);
    if (key_c == -1) {
        printf("error:%s\n", strerror(errno));
        key_c = IPC_PRIVATE;
    } 

    int msgid_c = msgget(key_c, IPC_CREAT | 0600);
    if (msgid_c == -1) {
        printf("error:%s\n", strerror(errno));
        return 1;
    } 
#endif
    key_t key_s = ftok(SPATH, SID);
    if (key_s == -1) {
        printf("error:%s\n", strerror(errno));
        key_s = IPC_PRIVATE;
    } 

    int msgid_s = msgget(key_s, IPC_CREAT | 0600);
    if (msgid_s == -1) {
        printf("error:%s\n", strerror(errno));
        return 1;
    } 

    msgctl(msgid_s, IPC_STAT, &msg_buf);
    printf("msgid_s = %d\n", msgid_s);

    msg.mtype = 1;
    len = sizeof(msg) - sizeof(4);
    while (1)
    {
        memset(&msg, 0, sizeof(struct mymsg));
        msg.mtype = 1;
        ret = msgrcv(msgid_s, &msg, len, msg.mtype, MSG_NOERROR);
        if (ret == -1) {
            perror("msgrcv");
            return 1;
        }

        printf("recv from :%s\n", msg.mydate);
        printf("please input your value: ");
        fflush(stdout);
        ret = read(0, buf, 256);
        buf[ret - 1] = '\0';
        memset(&msg, 0, sizeof(struct mymsg));
        msg.mtype = 2;
        memcpy(msg.mydate, buf, ret);
        ret = msgsnd(msgid_s, &msg, len, MSG_NOERROR);
        if (ret == -1) {
            perror("msgsnd");
            return 1;
        }
#if 0
        printf("Maximum number of bytes allowed in queue: %d\n", msg_buf.msg_qbytes);
        printf("Current number of messages in queue: %d\n", msg_buf.msg_qnum);
        printf("Key supplied to msgget: %d\n", msg_buf.msg_perm.__key);
        printf("Effective UID of owner: %d\n", msg_buf.msg_perm.uid);
        printf("Effective GID of owner: %d\n", msg_buf.msg_perm.gid);
        printf("Effective UID of creator: %d\n", msg_buf.msg_perm.cuid);
        printf("Effective GID of creator: %d\n", msg_buf.msg_perm.cgid);
        printf("Permissions: %x\n", msg_buf.msg_perm.mode);
        printf("Sequence number:%d\n", msg_buf.msg_perm.__seq);
#endif
    }
    
    return 0;
}


