#include "common_util.h"

time_t get_timestr(char dest[], size_t dest_len, const char * format)
{
    if (!dest || !dest_len || !format){
        return 0;
    }   
    dest[0] = '\0';

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
    dest[0] = '\0';

    time_t now;
    struct tm *tm_now;

    time(&now);
    tm_now = localtime(&now);
    size_t s = strftime(dest, dest_len, format, tm_now);

    uint64_t millSecond = GetMilliSecond();

    snprintf(dest + s, dest_len - s, ".%03d", (int)(millSecond %1000));

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

char* im_chomp(char *data) 
{
    char *ptr;

    if (data == NULL) {
        return NULL;
    }

    ptr = strchr(data, '\n');

    if (ptr != NULL) {
        *ptr-- = 0;

        if (ptr >= data && *ptr == '\r') {
            *ptr = 0;
        }
    }

    return data;
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


int GetStringByLabel(const std::string &sSrc,const std::string &sLabel1,const std::string &sLabel2, std::string &sOut, unsigned int nBeginPos, int nIfRetPos)
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
        sOut=std::string(pTmp0+sLabel1.length(), pTmp1-pTmp0-sLabel1.length());
    }
    else
    {
        sOut = std::string(pTmp0+sLabel1.length());
        ret=sSrc.length();
    }
    if (nIfRetPos == 0)
    {
        ret = 0;
    }
    return ret;
}


int GetCaseStringByLabel(const std::string &sSrc,const std::string &sLabel1,const std::string &sLabel2, std::string &sOut, unsigned int nBeginPos, int nIfRetPos)
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
        sOut=std::string(pTmp0+sLabel1.length(), pTmp1-pTmp0-sLabel1.length());
    }
    else
    {
        sOut = std::string(pTmp0+sLabel1.length());
        ret=sSrc.length();
    }
    if (nIfRetPos == 0)
    {
        ret = 0;
    }
    return ret;
}

std::string  StringTrim(std::string &sSrc)
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

    return sSrc;
}

std::string trim(const char * source, char const* delims)
{
    std::string result(source);
    std::string::size_type index = result.find_last_not_of(delims);
    if(index != std::string::npos)
        result.erase(++index);

    index = result.find_first_not_of(delims);
    if(index != std::string::npos)
        result.erase(0, index);
    else
        result.erase();      

    return result;
}

int stringStrip(const char *srcStr, const char *delim, std::string *dest, int s_mode)
{
    if (!srcStr || !dest)
    {
        return -1;
    }


    char *pstart = const_cast<char *>(srcStr);
    char *pend = strlen(srcStr) + pstart;
    size_t dlen = strlen(delim);

    if (!delim) {
        dest->append(pstart);
        return 0;
    }

    char *start = pstart;
    char *end = pend;

    while (s_mode & STRIP_MODE_LEFT)
    {
        if (!strncmp(pstart, delim, dlen))
        {
            pstart += dlen;
            start = pstart;
            //printf("start:%s\n", start);
        }else
            break;
    }

    while (s_mode & STRIP_MODE_RIGHT && pend - start >= (int)dlen)
    {
        pend = pend - dlen;
        if (!strncmp(pend, delim, dlen))
        {
            end = pend;
        }else
            break;
    }

    dest->clear();
    dest->append(start, end);

    return 0;
}

int SplitString(const char *srcStr, const char *delim, std::vector<std::string> * strVec, int s_mode)
{
    if (!srcStr || !delim || !strVec)
    {
        return 0;
    }

    strVec->clear();

    char *pstart = const_cast<char *>(srcStr);
    char * pend = NULL;
    int count = 0;

    size_t dlen = strlen(delim);
    while (1)
    {
        pend = strstr(pstart, delim);
        if (pend)
        {
            std::string str(pstart, pend);
            pstart =  pend + dlen;

            if (s_mode & SPLIT_MODE_TRIM)
            {
                if (!strncmp(str.c_str(), delim, dlen))
                    continue;
            }

            strVec->push_back(str);
            count += 1;
            if (SPLIT_MODE_ONE & s_mode)
            {
                break;
            }
        }
        else 
        {
            break;
        }
    }

    if (*pstart != '\0' && pstart != srcStr)
    {
        std::string str(pstart);
        strVec->push_back(str);
        count += 1;
    }

    return count;
}

int SplitString(const char *srcStr, char delim, std::vector<std::string> * strVec, int s_mode)
{
    if (!srcStr || !delim || !strVec)
    {
        return 0;
    }

    strVec->clear();

    char *pstart = const_cast<char *>(srcStr);
    char * pend = NULL;
    int count = 0;

    size_t dlen = sizeof(delim);
    while (1)
    {
        pend = strchr(pstart, delim);
        if (pend)
        {
            std::string str(pstart, pend);
            pstart =  pend + dlen;

            if (s_mode & SPLIT_MODE_TRIM)
            {
                if (str.length() == dlen && str[0] == delim)
                    continue;
            }

            strVec->push_back(str);
            count += 1;
            if (SPLIT_MODE_ONE & s_mode)
            {
                break;
            }
        }
        else 
        {
            break;
        }
    }

    if (*pstart != '\0' && pstart != srcStr)
    {
        std::string str(pstart);
        strVec->push_back(str);
        count += 1;
    }

    return count;
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


int UrlEncode(const std::string &sSrc, std::string &sDest)
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


int UrlDecode(const std::string &sSrc, std::string &sDest)
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


std::string GetMonStr(int nMonth)
{
    std::string sMonth;
    if (nMonth >=1 && nMonth <=12)
    {
        sMonth = std::string(MONTHARRAY[nMonth-1].sMonth);
    }
    return sMonth;
}

std::string GetWeekStr(int nWeek)
{
    std::string sWeek;
    if (nWeek >=0 && nWeek <=6)
    {
        sWeek = std::string(WEEKARRAY[nWeek].sWeek);
    }
    return sWeek;
}


std::string SecToHttpTime(time_t tmpTime)
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
    return std::string(sTime);
}

int parse_domain(const std::string &sDomain, std::vector<std::string> & vIp)
{
    char ipbuf[SIZE_LEN_16];

    int ret;
    struct addrinfo *res, *cur;
    struct sockaddr_in *addr;

    struct addrinfo hint;
    bzero(&hint, sizeof(hint));
    hint.ai_family = AF_INET;
    hint.ai_socktype = SOCK_STREAM;
    std::set<std::string> ss;

    ret = getaddrinfo(sDomain.c_str(), NULL, NULL, &res);
    if (-1 == ret)
    {
        fprintf(stderr, "%s\n", gai_strerror(ret));
        return -1;
    }

    for (cur = res; cur != NULL; cur = cur->ai_next)
    {
        addr = (struct sockaddr_in *)cur->ai_addr;
        inet_ntop(AF_INET, &addr->sin_addr, ipbuf, sizeof(ipbuf));
        ss.insert(ipbuf);
    }
    vIp.assign(ss.begin(), ss.end());

    freeaddrinfo(res);

    return 0;
}

int parse_url(const std::string &url, url_info & info)
{
    info.url.append(url);
    std::vector<std::string> tmp_vec;
    SplitString(url.c_str(), "://", &tmp_vec, SPLIT_MODE_ONE);
    if (tmp_vec.size()) 
    {
        info.protocol_type = tmp_vec[0];
    }

    std::vector<std::string> t_vec;
    SplitString(tmp_vec[1].c_str(), "/", &t_vec, SPLIT_MODE_ONE);
    info.full_path.append("/");
    info.path.append("/");
    if (t_vec.size()) 
    {
        tmp_vec.clear();
        SplitString(t_vec[0].c_str(), ":", &tmp_vec, SPLIT_MODE_ONE);
        if (tmp_vec.size()) 
        {
            info.port = atoi(tmp_vec[1].c_str());
            info.domain = tmp_vec[0];
        }
        else 
        {
            info.domain = t_vec[0];
            if (info.protocol_type == "http")
                info.port = 80;
            else if (info.protocol_type == "https")
                info.port = 443;
        }

        tmp_vec.clear();

        info.full_path.append(t_vec[1]);

        SplitString(t_vec[1].c_str(), "?", &tmp_vec, SPLIT_MODE_ONE);
        if (tmp_vec.size()) 
        {
            info.path.append(tmp_vec[0]);
            info.query.append(tmp_vec[1]);
        } 
        else 
        {
            info.path.append(t_vec[1]);
        }
    }

    return 0;
}
void parse_url_para(const std::string &url_path, std::map<std::string, std::string> &url_para_map)
{
    std::vector<std::map<std::string, std::string>> tmap;
    parse_url_para(url_path, tmap);
    for (auto ii = tmap.begin(); ii != tmap.end(); ii++)
    {
        url_para_map[ii->begin()->first] = ii->begin()->second;
    }
}

void parse_url_para(const std::string &url_path, std::vector<std::map<std::string, std::string>> &url_para_map)
{
    size_t pos = url_path.find("?");
    if (pos == std::string::npos)
    {
        return;
    }

    std::vector<std::string> vec_str;
    SplitString(url_path.substr(pos + 1).c_str(), "&", &vec_str, SPLIT_MODE_ALL);
    if (!vec_str.size())
    {
        vec_str.push_back(url_path.substr(pos + 1));
    }

    size_t num = vec_str.size();

    for (size_t ii = 0; ii < num; ii ++)
    {
        std::vector<std::string> tmp_vec;
        SplitString(vec_str[ii].c_str(), "=", &tmp_vec, SPLIT_MODE_ONE);
        if (tmp_vec.size() == 2)
        {
            std::map<std::string, std::string> tmap;
            StringTrim(tmp_vec[0]);
            StringTrim(tmp_vec[1]);
            std::string tmp_para;
            UrlDecode(tmp_vec[1], tmp_para);
            tmap.insert(std::make_pair(tmp_vec[0], tmp_para));
            url_para_map.push_back(tmap);
        }
    }               
}

int exec_shell_cmd(std::string & cmd, std::string & res)
{
    res.clear();
    FILE * stream;
    const int max_buffer = 256;
    char buffer[max_buffer];
    cmd.append(" 2>/dev/null");

    stream = popen(cmd.c_str(), "r");
    if (stream) {
        while (!feof(stream))
            if (fgets(buffer, max_buffer, stream) != NULL) res.append(buffer);
        pclose(stream);
    }   

    //printf("%s\n", res.c_str());

    return 0;
}

//这次返回的结果作为下次的种子
uint32_t common_random(uint32_t *seed)
{
    uint32_t r;
    r = *seed = *seed * 1103515245 + 12345;
    return (r << 16) | ((r >> 16) & 0xffff);
}

int dayofweek(int d, int m, int y)
{
    int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
    y -= m < 3;
    return ( y + y/4 - y/100 + y/400 + t[m-1] + d) % 7;
}

int yisleap(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int get_yday(int mon, int day, int year)
{
    static const int days[2][13] = {
        {0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334},
        {0, 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335}
    };
    int leap = yisleap(year);

    return days[leap][mon] + day;
}

//参考网络
int date_change(const std::string &in, int diff, std::string &out)
{
    out.clear();

    time_t t1;
    struct tm tmin;
    struct tm *tminp=&tmin;
    struct tm tmout;
    struct tm *tmoutp=&tmout;

    memset(tminp, 0, sizeof(struct tm));
    memset(tmoutp, 0, sizeof(struct tm));

    char tmp[SIZE_LEN_512];
    memset(tmp,0,sizeof(tmp));
    strptime(in.c_str(),"%Y%m%d %Z",tminp);

    t1 = mktime(tminp);
    t1 += diff*60*60*24;

    tmoutp=localtime(&t1);

    strftime(tmp,sizeof(tmp),"%Y%m%d",tmoutp);
    out.append(tmp);

    return 0;
}

bool start_with(const std::string &mainStr, const std::string &toMatch)
{
    if(mainStr.size() >= toMatch.size() &&
            mainStr.compare(0, toMatch.size(), toMatch) == 0)
        return true;
    else
        return false;
}

bool end_with(const std::string &mainStr, const std::string &toMatch)
{
    if(mainStr.size() >= toMatch.size() &&
            mainStr.compare(mainStr.size() - toMatch.size(), toMatch.size(), toMatch) == 0)
        return true;
    else
        return false;
}

void get_yearweek(const std::string &in, std::string &out)
{

    out.clear();

    struct tm tmin;
    struct tm *tminp=&tmin;

    memset(tminp, 0, sizeof(struct tm));

    char tmp[SIZE_LEN_512];
    memset(tmp,0,sizeof(tmp));
    strptime(in.c_str(),"%Y%m%d %Z",tminp);

    strftime(tmp,sizeof(tmp),"%Y%W",tminp);
    out.append(tmp);
}
