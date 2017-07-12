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
#include <dirent.h>
#include <fcntl.h>
#include <stdlib.h>

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
#include <deque>
//#include <strstream>
#include <sstream>
#include <exception>
#include <memory>


using namespace std;

//using namespace com::mingz::frame;


#define DAFAULT_EPOLL_SIZE 1000
#define DEFAULT_EPOLL_WAITE 20


#define LOG_DATE_FORMAT "%Y-%m-%d %H:%M:%S"


const size_t SEND_BUF_SIZE = 20 * 1024;
const uint64_t CONNECT_TIME_OUT = 180 * 1000;

const int MAX_RECV_SIZE = 1024*20;
const int MAX_SEND_NUM = 5;

const uint32_t MAX_HTTP_HEAD_LEN = 100*1024;


#define MAX_BUF_SIZE 1024 * 10 * 10

#define DEFAULT_LOG_BUCKETLEN 200



/******** 长度定义 *************/

static const uint32_t SIZE_LEN_16 = 16; 
static const uint32_t SIZE_LEN_32 = 32; 
static const uint32_t SIZE_LEN_64 = 64; 
static const uint32_t SIZE_LEN_128 = 128; 
static const uint32_t SIZE_LEN_256 = 256;
static const uint32_t SIZE_LEN_512 = 512;
static const uint32_t SIZE_LEN_1024 = 1024;
static const uint32_t SIZE_LEN_2048 = 2048;
static const uint32_t SIZE_LEN_4096 = 4096;
static const uint32_t SIZE_LEN_8192 = 8192;
static const uint32_t SIZE_LEN_16384 = 16384;
static const uint32_t SIZE_LEN_32768 = 32768;
static const uint32_t SIZE_LEN_65536 = 65536;


/***************************/


typedef signed char  int8_t;
typedef unsigned char  uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned int uint32_t;



/*****************************/
#define DEFAULT_LOG_MAX_SIZE 50*1024*1024


const int m_prime_list[] = {
    1009,   2003,   3001,   4001,   5003,   6007,   7001,   8009,   9001,
    10007,  20011,  30011,  40009,  50021,  60013,  70001,  80021,  90001,
    100003, 200003, 300007, 400009, 500009, 600011, 700001, 800011, 900001,
    1000003,2000003,3000017,4000037,5000011,6000011,7000003,8000009,9000011,
    12582917,   25165843,   50331653,   100663319,
    201326611,  402653189,  805306457,  1610612741
};

#define CHANNEL_MSG_TAG "c"

#endif
