#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define DAY (24*60*60)

time_t getYesterday(char *dst, struct tm *stm)
{
    struct tm *res;
    char buf[32];
    time_t timer;

    timer = time(NULL);
    timer = timer - DAY;

    res = localtime_r(&timer, stm);
    if (NULL == res){
        perror("localtime");
        exit(1);
    }

    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", res);
    //sprintf(dst, "%d-%d-%d %02d:%02d:%02d", stm->tm_year + 1900, stm->tm_mon + 1, stm->tm_mday, stm->tm_hour, stm->tm_min, stm->tm_sec);
    memcpy(dst, buf, strlen(buf)+1);
    return mktime(stm);
}


void getStrTime(time_t cur, char *buf, int len)
{
    struct tm * res;

    res = localtime(&cur);
    if (NULL == res)
    {
        perror("localtime");
        exit(1);
    }

    strftime(buf, len, "%Y-%m-%d %H:%M:%S", res);
}




int main(void)
{
    int digit;
    char sbuf[32], ebuf[32], tmp[32];
    struct tm stm, ctm;
    time_t timer;
    

    getYesterday(sbuf, &stm);
    strftime(tmp, sizeof(tmp), "%W", &stm);
    digit = atoi(tmp);
    ctm =stm;

    ctm.tm_wday = 0;
    timer = mktime(&ctm); 
    getStrTime(timer, ebuf, sizeof(ebuf));


    printf("%s, %s\n", sbuf, ebuf);

    return 0;
}
