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

#include <netinet/tcp.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include <arpa/inet.h>

#include <netdb.h>

#include <unistd.h>

#include <openssl/sha.h>
#include <libgen.h>


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
#include <fstream>

#include <unordered_set>
#include <unordered_map>
#include <functional>
#include <mutex>
#include <stdexcept>


#define DAFAULT_EPOLL_SIZE 1000
#define DEFAULT_EPOLL_WAITE 10
#define DEFAULT_EPOLL_WAITE_SIZE 10


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

const int URLESCAPE[] =
{
    //0   1   2   3   4   5   6   7   8   9   10 11 12 13 14  15
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, //0~15 
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,//16~31
        1,  0,  1,  1,  1,  1,  1,  0,  0,  0,  0,  1,  1,  0,  0,  1,//32~47
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,//48~63
        1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,//64~79
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  0,//80~95
        1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,//96~111
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  0,//112~127
        1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,//128~143
        1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,//144~150
        1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,//160~175
        1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,//176~191
        1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,//1192~207
        1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,//208~223
        1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,//224~239
        1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1//240~255
};
/** obj_id **/
#define OBJ_ID_THREAD 1
#define OBJ_ID_DOMAIN 2



/** 外边业务用起始的obj id **/
#define OBJ_ID_BEGIN 1000

/** 外边业务用起始的timer id **/
#define TIMER_ID_BEGIN 1000

/** 域名缓存默认超时时间 单位是毫秒***/
#define MAX_DOMAIN_TIMEOUT (3 * 60 * 60 * 1000)

#define CRLF "\r\n"
#define CRLF2 "\r\n\r\n"


/** normal_msg op**/
#define NORMAL_MSG_CONNECT 1


/*** timer type ***/
#define NONE_TIMER_TYPE 1
#define DELAY_CLOSE_TIMER_TYPE 2
#define WEB_SOCKET_HANDSHAKE_OK_TIMER_TYPE 3
#define DOMAIN_CACHE_TIMER_TYPE 5


#endif
