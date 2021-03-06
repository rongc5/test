#ifndef __COMMON_DEF_H__
#define __COMMON_DEF_H__

#include "base_def.h"


enum CONNECT_STATUS
{
    CLOSED = 0,
    CONNECT_OK = 1,
    CONNECTING = 2  
};

enum EPOLL_TYPE
{
    EPOLL_LT_TYPE = 0,
    EPOLL_ET_TYPE 
};


enum RECV_MSG_STATUS
{
    RECV_MSG_HEAD = 0,
    RECV_MSG_BODY 
};

#ifdef DEBUG
#define PDEBUG(format, arg...) \
    do { \
        printf("tid:[%lu],line:[%d],func:[%s],file:[%s] "format, pthread_self(), __LINE__, __func__, __FILE__, ##arg); \
    } while (0)
#else
#define PDEBUG(format, arg...)
#endif


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



time_t get_date_str(char dest[], size_t dest_len, const char * format);

uint64_t GetMilliSecond();


enum LogType {
    LOGOFF,
    LOGFATAL,
    LOGWARNING,
    LOGNOTICE,
    LOGTRACE,
    LOGDEBUG,
    LOGSIZE
};


void get_proc_name(char buf[], size_t buf_len);

struct log_conf{
    uint32_t file_max_size;
    char log_path[SIZE_LEN_256];
    char prefix_file_name[SIZE_LEN_256];
    LogType type;
    int deal_mode; // 1 write log 2 print log 3 write && print

    log_conf()
    {
        file_max_size = DEFAULT_LOG_MAX_SIZE;
        strcpy(log_path, "logs");
        get_proc_name(prefix_file_name, sizeof(prefix_file_name));
        type = LOGDEBUG;
        deal_mode = 1;
    }
};

template<class OBJ>
class REC_OBJ
{
    public:
        REC_OBJ(OBJ * p_obj):_p_obj(p_obj){}
        ~REC_OBJ(){
            if (_p_obj){
                delete _p_obj;
            }
        }
    private:
        OBJ *_p_obj;
};


void set_unblock(int fd);

struct ObjId
{
    uint32_t _id;
    uint32_t _thread_index;

    ObjId():_id(0), _thread_index(0){}
};

bool operator < (const ObjId & oj1, const ObjId & oj2);

bool operator==(const ObjId & oj1, const ObjId & oj2);

enum passing_msg_op
{
    PASSING_FD,
    PASSING_LOG,
    PASSING_REQ_HTTP,
    PASSING_RES_HTTP,
    PASSING_MSG
};

struct base_passing_msg  //内部传递的消息
{
    ObjId _dst_id;
    ObjId _src_id;
    passing_msg_op _op;
    uint64_t _start_time;
    base_passing_msg(){
        _start_time = GetMilliSecond();
    }
    virtual void release(){
        delete this;
    }

    virtual ~base_passing_msg(){}
};


struct recv_msg_fd: public base_passing_msg
{
    int fd;
    recv_msg_fd():fd(0){}

    virtual ~recv_msg_fd(){}
};


struct log_msg : public base_passing_msg {
    LogType type; 
    string str;
    pthread_t tid;

    log_msg():type(LOGOFF), tid(0){}
};


enum HTTP_MODE
{   
    HTTP_GET,
    HTTP_POST
};

struct http_req_msg: public base_passing_msg
{
    HTTP_MODE http_mode;
    string  url;
    string post_data;
    map<string, string> headers;
};

struct http_res_msg:public base_passing_msg
{
    int res_code;
    string res_buf;
    struct evhttp_uri *uri;
    struct evhttp_connection *cn;
    http_res_msg():res_code(0), uri(NULL), cn(NULL){}

    virtual ~http_res_msg() {
        if (uri) { 
            evhttp_uri_free(uri);
            uri = NULL;
        }

        if (cn) {
            evhttp_connection_free(cn);
            cn = NULL;
        }


    }
};

uint64_t htonl64(uint64_t src);

uint64_t ntohl64(uint64_t src);


int GetCaseStringByLabel(const string &sSrc,const string &sLabel1,const string &sLabel2,
                    string &sOut, unsigned int nBeginPos=0, int nIfRetPos=0);

int GetStringByLabel(const string &sSrc,const string &sLabel1,const string &sLabel2,
                    string &sOut, unsigned int nBeginPos=0, int nIfRetPos=0);

void StringTrim(string &sSrc);

void StringTrim(string &sSrc);

int SplitString(const char *srcStr, string &delim, vector<string>& strList);

int SplitString(const string &srcStr,const string &delim, vector<string>& strList);

int get_prime_num(int num);

#endif
