#ifndef __BASE_DEF_H__
#define __BASE_DEF_H__


#include <sys/socket.h>
#include <sys/epoll.h>
#include <pthread.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdarg.h>
#include <sys/time.h>

#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include <arpa/inet.h>

#include <unistd.h>


#include <vector>
#include <string>
#include <list>
#include <map>
#include <set>
#include <strstream>
#include <sstream>
#include <exception>
#include <memory>


using namespace std;


#define DAFAULT_EPOLL_SIZE 1000
#define DEFAULT_EPOLL_WAITE 20


#define MSG_HEAD_BODY_LENTH_LEN 4



#define LOG_DATE_FORMAT "%Y-%m-%d %H:%M:%S"



const size_t SEND_BUF_SIZE = 20 * 1024;
const uint64_t CONNECT_TIME_OUT = 180 * 1000;

const int max_recv_data = 1024*20;
const int max_send_num = 5;





/******** 长度定义 *************/

static const uint32_t SIZE_LEN_16 = 16; 
static const uint32_t SIZE_LEN_32 = 32; 
static const uint32_t SIZE_LEN_64 = 64; 
static const uint32_t SIZE_LEN_128 = 128; 
static const uint32_t SIZE_LEN_256 = 256;



/***************************/


typedef signed char  int8_t;
typedef unsigned char  uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned int uint32_t;



/***************************/

#define PASSING_ACCEPT_NONE 0
#define PASSING_ACCEPT_IN 1


/***************************/



#define ASSERT_DO(valid, PRINT) \
    do{ \
        if (!(valid)) { \
            (PRINT); \
            return -1; \
        } \
    }while(0)

#define ASSERT(valid) \
    do { \
        if (!(valid)) { \
            return -1; \
        } \
    }while(0)



/*****************************/


#if DEBUG
#define PDEBUG(format, arg...) printf("line:[%d],func:[%s],file:[%s] "format, __LINE__, __func__, __FILE__, ##arg)
#else
#define PDEBUG(format, arg...) do while(0)
#endif



#endif
