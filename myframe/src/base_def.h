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

#include <sys/stat.h>
#include <unistd.h>
#include <string.h>


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


const size_t SEND_BUF_SIZE = 20 * 1024;
const uint64_t CONNECT_TIME_OUT = 180 * 1000;

const int max_recv_data = 1024*20;
const int max_send_num = 5;





/******** 长度定义 *************/

#define SIZE_LEN_64 64




#endif
