#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>

struct msgbuf {
    long mtype;     /* message type, must be > 0 */
    char mtext[5];  /* message data */
};



int main(void)
{
    int msqid;
    struct msqid_ds info;
    struct msgbuf buf;

    msqid = msgget(IPC_PRIVATE, 0600 | IPC_CREAT);

    buf.mtype = 1;
    buf.mtext[0] = 1;
    msgsnd(msqid, &buf, 1, 0);

    msgctl(msqid, IPC_STAT, &info);
    printf("read-write: %03o, cbytes = %lu, qnum = %lu, qbytes = %lu\n",\ 
            info.msg_perm.mode&0777, (unsigned long)info.msg_cbytes,\
            (unsigned long)info.msg_qnum, (unsigned long)info.msg_qbytes);

    system("ipcs -q");

    msgctl(msqid, IPC_RMID, NULL);
    return 1;
}

