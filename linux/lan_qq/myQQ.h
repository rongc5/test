#ifndef __MYQQ__H
#define __MYQQ__H

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <sched.h>
#include <dirent.h>


#include <signal.h>
#include <poll.h>
#include <errno.h>


#include <time.h>


#define SERVERIPADDR "0.0.0.0"

#define SERVERPORT 8080
#define CHATPORT 8888
#define NAMESIZE 12
#define MAXSIZE 256
#define IPSIZE 16
#define JAR "99"
#define MYQQ_LOG_DB "myQQ_login_data.db"


#define DEBUG
#ifdef DEBUG
#define PDEBUG(format, arg...) printf("QF:"format, ##arg)
#else
#define PDEBUG(format, arg...) do while(0)
#endif

typedef enum {
    OFFLINE,//不在线	
    ONLINE,
    HIDDEN,
    AWAY,//离开
} MYQQ_Status;

typedef enum myQQ_qd
{
    IN,
    OUT
}MYQQ_QD;

typedef struct myQQ_sd_info
{
    int sd;
    struct myQQ_ldata * phead;
}MYQQ_SD_INFO;

typedef struct myQQ_ldata
{
    char mylog_name[NAMESIZE];
    char mylog_passwd[MAXSIZE];
    char mylog_nickname[NAMESIZE];
    char mylog_ipbuf[IPSIZE];
    char mylog_msgbox[MAXSIZE];
    MYQQ_Status mylog_status;
    MYQQ_QD mylog_qd;
    int flag;
    struct myQQ_ldata *next;
}MYQQ_LDATA;

void display_myQQ_login(MYQQ_LDATA * phead, MYQQ_LDATA rd, int sd, struct sockaddr_in userend);
void display_myQQ_regester(MYQQ_LDATA * phead, MYQQ_LDATA rd, int sd, struct sockaddr_in userend);

char buf_logname[22];


#endif
