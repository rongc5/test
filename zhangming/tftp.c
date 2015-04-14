#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define NAMELENMAX 512
#define DATASIZE 512
typedef unsigned short int u_short;

enum
{
    RRQ,
    WRQ,
    DATA,
    ACK
};

typedef struct tftpheadr
{
    u_short opcode;
    union
    {
        char filename[NAMELENMAX];
        struct
        {
            u_short seq;
            char data[DATASIZE];
        }filedata;
        u_short seq;

    }opdata;
}tftphdr


