#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define KEYPATH  "/etc/passwd"
#define KEYID 88
#define DEBG 0

int main(void)
{
    key_t key;
    int msgid;
#if DEBG 
    key = ftok(KEYPATH, KEYID);
    {}
#else
    key = IPC_PRIVATE;
#endif
   // msgid = msgget(key, IPC_CREAT | 0600);
    msgid = msgget(key, 0600);
    {}

    printf("key = 0x%x, msgid = %d\n", key, msgid);

    return 0;

}

