#ifndef _HEAD_H
#define _HEAD_H

#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 4000

typedef enum
{
    FULL,
    NORMAL,
    EMPTY
}STATUS;

typedef enum
{
    READ,
    WRITE
}RDWRFLAG;

typedef struct
{
    char buf[BUFSIZE];
    int head;
    int tail;
    STATUS state;
    pid_t rpid;
    pid_t wpid;
}PIPE;

int creatPipe(PIPE **pipe);
void setRDWRflag(PIPE *pipe, RDWRFLAG flag);

int readPipe(PIPE * pipe, char * buf, int size);
int writePipe(PIPE * pipe, const char * buf, int size);
int closePipe(PIPE * pipe);


#endif
