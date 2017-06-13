#include "common_def.h"
#include "log_helper.h"


time_t get_date_str(char dest[], size_t dest_len, const char * format)
{
    if (!dest || !dest_len || !format){
        return 0;
    }   

    time_t now;
    struct tm *tm_now;

    time(&now);
    tm_now = localtime(&now);
    strftime(dest, dest_len, format, tm_now);

    return now;
}


uint64_t GetMilliSecond()
{
    timeval tm; 
    gettimeofday(&tm, NULL);
    uint64_t now = tm.tv_sec * 1000 + tm.tv_usec/1000;
    return now;
}

void get_proc_name(char buf[], size_t buf_len) {

    if (!buf || !buf_len) {
        return;
    }

    pid_t pid = getpid();
    char exec_file[SIZE_LEN_1024];
    char proc_name[SIZE_LEN_1024];
    snprintf(exec_file, sizeof(exec_file), "/proc/%d/exe", pid);
    int ret = readlink(exec_file, proc_name, sizeof(proc_name));
    if (-1 == ret)
    {
        //LOG_WARNING("readlink for proc_name error, pid:%d", pid);
        buf[0] = 0;
        return;
    }
    proc_name[ret] = 0;
    //获取文件名 
    char *p = strrchr(proc_name, '/');
    if (NULL == p)
    {
        buf[0] = 0;
        return;
    }
    snprintf(buf, buf_len, "%s", p + 1);
    return;
}

void set_unblock(int fd)
{
    int ret = fcntl(fd, F_GETFL);

    fcntl(fd, F_SETFL, ret | O_NONBLOCK);
}

bool operator<(const ObjId & oj1, const ObjId & oj2)
{
    if (oj1._id != oj2._id){
        return oj1._id < oj2._id;
    } else if (oj1._thread_index != oj2._thread_index) {
        return oj1._thread_index < oj2._thread_index;
    }else 
        return false;
}


bool operator==(const ObjId & oj1, const ObjId & oj2)
{
    return oj1._thread_index ==  oj2._thread_index && oj1._id == oj2._id;
}

uint64_t htonl64(uint64_t src)
{
    uint64_t uiRet = src;
#if __BYTE_ORDER == __LITTLE_ENDIAN
    uiRet = __bswap_64(src);
#endif
    return uiRet;
}

uint64_t ntohl64(uint64_t src)
{
    uint64_t uiRet = src; 
#if __BYTE_ORDER == __LITTLE_ENDIAN
    uiRet = __bswap_64(src);
#endif
    return uiRet;
}

int GetStringByLabel(const string &sSrc,const string &sLabel1,const string &sLabel2, string &sOut, unsigned int nBeginPos, int nIfRetPos)
{
    if (nBeginPos >= sSrc.length())
    {
        return -1;
    }
    char *pTmp = (char*)sSrc.c_str() + nBeginPos;
    char *pTmp0;
    char *pTmp1;
    if (sLabel1 == "")
    {
        pTmp0 = pTmp;
    }
    else
    {  
        pTmp0= strcasestr(pTmp, sLabel1.c_str());
        if (pTmp0 == NULL)
        { 
            return -1;
        }
    }
    int ret=pTmp0-sSrc.c_str()+sLabel1.length();
    if (sLabel2 != "")
    {   
        pTmp1 = strcasestr(pTmp0+sLabel1.length(), sLabel2.c_str());
        if (pTmp1 == NULL)
        {
            return -1;
        }

        ret=pTmp1+sLabel2.length()-sSrc.c_str();
        sOut=string(pTmp0+sLabel1.length(), pTmp1-pTmp0-sLabel1.length());
    }
    else
    {
        sOut = string(pTmp0+sLabel1.length());
        ret=sSrc.length();
    }
    if (nIfRetPos == 0)
    {
        ret = 0;
    }
    return ret;
}


int GetCaseStringByLabel(const string &sSrc,const string &sLabel1,const string &sLabel2, string &sOut, unsigned int nBeginPos, int nIfRetPos)
{
        if (nBeginPos >= sSrc.length())
        {
            return -1;
        }
        char *pTmp = (char*)sSrc.c_str() + nBeginPos;
        char *pTmp0;
        char *pTmp1;
        if (sLabel1 == "")
        {
            pTmp0 = pTmp;
        }
        else
        {  
            pTmp0= strcasestr(pTmp, sLabel1.c_str());
            if (pTmp0 == NULL)
            { 
                return -1;
            }
        }
        int ret=pTmp0-sSrc.c_str()+sLabel1.length();
        if (sLabel2 != "")
        {   
            pTmp1 = strcasestr(pTmp0+sLabel1.length(), sLabel2.c_str());if (pTmp1 == NULL)
            {
                return -1;
            }
            ret=pTmp1+sLabel2.length()-sSrc.c_str();
            sOut=string(pTmp0+sLabel1.length(), pTmp1-pTmp0-sLabel1.length());
        }
        else
        {
            sOut = string(pTmp0+sLabel1.length());
            ret=sSrc.length();
        }
        if (nIfRetPos == 0)
        {
            ret = 0;
        }
        return ret;
}

void StringTrim(string &sSrc)
{
    int i = 0;
    while ((sSrc[i] == ' ' || sSrc[i] == '\t' || sSrc[i] == '\r' || sSrc[i] == '\n') && i < (int)sSrc.length())
    {
        i++;
    }
    int nBeginPos = i;
    i = sSrc.length() - 1;
    while ((sSrc[i] == ' ' || sSrc[i] == '\t' || sSrc[i] == '\r' || sSrc[i] == '\n') && i >=0)
    {
        i--;
    }
    int nEnd = i;
    sSrc = sSrc.substr(nBeginPos, nEnd - nBeginPos + 1); 
}

