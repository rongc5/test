#ifndef __COMMON_UTIL_H__
#define __COMMON_UTIL_H__

#include "base_def.h"
#include "common_def.h"
#include "log_helper.h"

void get_proc_name(char buf[], size_t buf_len);

#define DEL_PTR(PTR) \
    do {\
        delete PTR; \
        PTR = NULL;\
    }while (0)

#define DEL_ARR(PTR) \
    do {\
        delete [] PTR; \
        PTR = NULL;\
    }while (0)

int get_prime_num(int num);

/*
 *去除字符串末尾的单个回车换行符 ("\n" 或者 "\r\n")
 *
 *@data - 待处理的字符串;
 *
 *总是返回 @data;
 *
 */
char* im_chomp(char *data);

#define STRIP_MODE_LEFT 0x1  
#define STRIP_MODE_RIGHT 0x2

int stringStrip(const char *srcStr, const char *delim, std::string *dest, int s_mode);

#define SPLIT_MODE_ONE 0x1
#define SPLIT_MODE_ALL 0x2
int SplitString(const char *srcStr, const char *delim, std::vector<std::string> * strVec, int s_mode);

void StringTrim(std::string &sSrc);

std::string trim(const char * source, char const* delims = " \t\r\n");

int GetCaseStringByLabel(const std::string &sSrc,const std::string &sLabel1,const std::string &sLabel2, std::string &sOut, unsigned int nBeginPos = 0, int nIfRetPos = 0);

int GetStringByLabel(const std::string &sSrc,const std::string &sLabel1,const std::string &sLabel2, std::string &sOut, unsigned int nBeginPos, int nIfRetPos);

uint64_t ntohl64(uint64_t src);

uint64_t htonl64(uint64_t src);

void set_unblock(int fd);

uint64_t GetMilliSecond();

time_t get_timestr(char dest[], size_t dest_len, const char * format);

time_t get_timestr_millSecond(char dest[], size_t dest_len, const char * format);

int UrlEncode(const std::string &sSrc, std::string &sDest);

int UrlDecode(const std::string &sSrc, std::string &sDest);

#define ASSERT_WARNING(valid, fmt, arg...) \
    do{ \
        if(!(valid)){ \
            LOG_WARNING(fmt, ##arg); \
            return -1; \
        } \
    }while(0)

#define ASSERT_DEBUG(valid, fmt, arg...) \
    do{ \
        if(!(valid)){ \
            LOG_DEBUG(fmt, ##arg); \
            return -1; \
        } \
    }while(0)


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
    
    
#ifdef DEBUG
#define PDEBUG(format, arg...) \
    do { \
        printf("tid:[%lu],line:[%d] ,func:[%s],file:[%s] " format, pthread_self(), __LINE__, __func__, __FILE__, ##arg); \
} while (0)
#else
    #define PDEBUG(format, arg...) 
#endif

#define FORMAT_STR(DST, SRC) \
    do {\
        std::stringstream ss;\
        ss << SRC; \
        DST = ss.str(); \
    }while (0)


char * strncasestr(const char *p, int nLen, const char *pattern);


std::string SecToHttpTime(time_t tmpTime);

struct TMonth
{       
    int nMonth;
    char sMonth[10];
};

struct TWeek
{
    int nWeek;
    char sWeek[10];
};

const TWeek WEEKARRAY[] =  
{   
    {0, "Sun"},
    {1, "Mon"},
    {2, "Tue"},
    {3, "Wed"},
    {4, "Thu"},
    {5, "Fri"},
    {6, "Sat"}
};  

const TMonth MONTHARRAY[]= 
{ 
    {1, "Jan"},
    {2, "Feb"},
    {3, "Mar"},
    {4, "Apr"},
    {5, "May"},
    {6, "Jun"},
    {7, "Jul"},
    {8, "Aug"},
    {9, "Sep"},     
    {10, "Oct"},
    {11, "Nov"},
    {12, "Dec"}
}; 


std::string GetMonStr(int nMonth);
std::string GetWeekStr(int nWeek);

int parse_domain(const std::string &sDomain, std::vector<std::string> & vIp);

int parse_url(const std::string &url, url_info & info);

void parse_url_para(const std::string &url_path, std::map<std::string, std::string> &url_para_map);

template <class T, class U>  
bool has_key(const std::map<T, U> & query, const T &key);

template <class T, class U>
bool has_key(const std::map<T, U> & query, const T & key)
{
    typename std::map<T, U>::const_iterator  it;
    it = query.find(key);
    if (it == query.end())
    {
        return false;
    }

    return true;
}

int exec_shell_cmd(std::string & cmd, std::string & res);

template<typename T>
class ToBufferMgr
{
    public:
        ToBufferMgr()
        {
            _curr = 0;
        }

        T* current()
        {
            if(_curr == 0 || _curr == 1)
            {
                return &(_objects[_curr]);
            }

            return NULL;
        }

        T * idle()
        {
            if(_curr == 0 || _curr == 1)
            {
                return &(_objects[1 - _curr]);
            }

            return NULL;
        }

        void idle_2_current()
        {
            if(_curr == 0 || _curr == 1)
            {
                _curr = 1 - _curr;
            }
        }

    private:
        T _objects[2];
        int16_t _curr;
};

#endif
