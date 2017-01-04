#ifndef __BASE_DEF_H__
#define __BASE_DEF_H__


#include <sys/socket.h>
#include <sys/epoll.h>
#include <string>
#include <list>
#include <map>
#include <pthread.h>
#include <vector>
#include <time.h>



using namespace std;


#define DAFAULT_EPOLL_SIZE 1000
#define DEFAULT_EPOLL_WAITE 20


#define MSG_HEAD_BODY_LENTH_LEN 4


const size_t SEND_BUF_SIZE = 20 * 1024;
const uint64_t CONNECT_TIME_OUT = 180 * 1000;

const int max_recv_data = 1024*20;
const int max_send_num = 5;



#endif
