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

#include <unistd.h>


#include <vector>
#include <string>
#include <list>
#include <map>
#include <strstream>
#include <sstream>
#include <exception>


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



#endif
