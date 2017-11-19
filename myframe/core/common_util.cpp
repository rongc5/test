#include "common_util.h"

time_t get_timestr(char dest[], size_t dest_len, const char * format)
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

 time_t get_timestr_millSecond(char dest[], size_t dest_len, const char * format)
{
    if (!dest || !dest_len || !format){
        return 0;
    }   

    time_t now;
    struct tm *tm_now;

    time(&now);
    tm_now = localtime(&now);
    size_t s = strftime(dest, dest_len, format, tm_now);

    uint64_t millSecond = GetMilliSecond();
    
    snprintf(dest + s, dest_len - s, ".%03d", millSecond %1000);

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
        buf[0] = 0;
        return;
    }
    proc_name[ret] = 0;

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

int SplitString(const char *srcStr, const string &delim, vector<string>& strList)
{
    char *sTmpBuf = new char[strlen(srcStr) + 1];
    char *tt = sTmpBuf;
    char *sToken = strtok_r((char*)srcStr, delim.c_str(), &tt);
    while (sToken)
    {
        strList.push_back(sToken);
        sToken = strtok_r(NULL, delim.c_str(),  &tt);
    }
    delete []sTmpBuf;
    return 0;
}

int SplitString(const string &srcStr,const string &delim, vector<string>& strList)
{
    char *sBuf = new char[srcStr.length() + 1];
    char *sTmpBuf = new char[srcStr.length() + 1];

    strncpy(sBuf, srcStr.c_str(), srcStr.length());
    sBuf[srcStr.length()] = 0;

    char *tmpSrc = sBuf;
    char *tt = sTmpBuf;

    char *sToken = strtok_r(tmpSrc, delim.c_str(), &tt);
    while (sToken)
    {
        strList.push_back(string(sToken));
        sToken = strtok_r(NULL, delim.c_str(), &tt);
    }
    delete [] sBuf;
    delete [] sTmpBuf;
    return 0;
}

int SplitfirstDelimString(const string &srcStr,const string &delim, vector<string>& strList)
{
    size_t pos = srcStr.find_first_of(delim);
    if (pos != std::string::npos) {
        string tmp_str = srcStr.substr(0, pos);
        string left_str = srcStr.substr(pos+delim.length());
        strList.push_back(tmp_str);
        strList.push_back(left_str);
    }

    return 0;
}


int get_prime_num(int num)
{
    const int prime_num = sizeof(m_prime_list)/sizeof(int);
    int turn = 0;
    for(turn = 0; turn < prime_num; ++turn){
        if(m_prime_list[turn] >= num){
            break;
        }
    }

    if(turn >= prime_num){
        return m_prime_list[prime_num - 1];
    }else{
        return m_prime_list[turn];
    }
}


int UrlEncode(const string &sSrc, string &sDest)
{
	    int nLen = sSrc.length();
    int i=0;
    while (i < nLen)
    {
        unsigned char tmpc = (unsigned char)sSrc[i];
        if (URLESCAPE[tmpc] == 1)
        {
            char sBuf[10] = {0};
            //sprintf(sBuf, "%c%02X", '%', tmpc);
			snprintf(sBuf, sizeof(sBuf),"%c%02X", '%', tmpc);
            sDest.append(sBuf);
        }
        else
        {
            sDest.append(1, sSrc[i]);
        }
        i++;
    }
    return 0;
}


int UrlDecode(const string &sSrc, string &sDest)
{
	int nLen = sSrc.length();
	for (int i = 0; i < nLen; )
	{
		unsigned char chTmp =  (unsigned char)sSrc[i];
		if (chTmp == '%')
		{
			if (i + 2 >= nLen)
				return -1;
			
		        ++i;
		        char szBuf[3] = {0};
		        szBuf[0] = sSrc[i++];
		        szBuf[1] = sSrc[i++];
		        int nTmp = strtoul(szBuf, 0, 16);
			 chTmp = nTmp;
		        sDest.append(1, chTmp);
		}
		else
		{
			sDest.append(1, sSrc[i]);
			++i;
		}
	}
	return 0;
}


string GetMonStr(int nMonth)
{
        string sMonth;
        if (nMonth >=1 && nMonth <=12)
        {
                sMonth = string(MONTHARRAY[nMonth-1].sMonth);
        }
        return sMonth;
}

string GetWeekStr(int nWeek)
{
        string sWeek;
        if (nWeek >=0 && nWeek <=6)
        {
                sWeek = string(WEEKARRAY[nWeek].sWeek);
        }
        return sWeek;
}


string SecToHttpTime(time_t tmpTime)
{
		struct timeval tm1;
        struct timezone tz1;        
        gettimeofday(&tm1, &tz1);
        tmpTime = tmpTime + tz1.tz_minuteswest*60;
        tm tmpTm;
        localtime_r(&tmpTime, &tmpTm);
        char sTime[128] = {0};
        //sprintf(sTime, "%s, %d %s %d %02d:%02d:%02d GMT ", GetWeekStr(tmpTm.tm_wday).c_str(),  tmpTm.tm_mday, 
        //        GetMonStr(tmpTm.tm_mon + 1).c_str(), 
        //        tmpTm.tm_year+1900, tmpTm.tm_hour, tmpTm.tm_min,   tmpTm.tm_sec);
		snprintf(sTime, sizeof(sTime),"%s, %d %s %d %02d:%02d:%02d GMT ", GetWeekStr(tmpTm.tm_wday).c_str(),  tmpTm.tm_mday, 
			GetMonStr(tmpTm.tm_mon + 1).c_str(), 
			tmpTm.tm_year+1900, tmpTm.tm_hour, tmpTm.tm_min,   tmpTm.tm_sec);
        return string(sTime);
}


char * strncasestr(const char *p, int nLen, const char *pattern)
{
    char *ret = NULL;
    char *pp = new char[nLen + 1];
    memcpy(pp, p, nLen);
    pp[nLen] = 0;
    char *pRet = strcasestr(pp, pattern);
    if (pRet != NULL)
    {
        ret = (char*)p + (pRet - pp);
    }
    delete[] pp;
    return ret;
}

