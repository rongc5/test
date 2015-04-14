#ifndef __IP_ADDR_INFO_
#define __IP_ADDR_INFO_
 


#include <sys/mman.h>
#include <fcntl.h>
#include <sys/types.h>
#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <errno.h> 




#define SHARE_MEMORY_FILE "./qqwry.dat" //共享内存路径.ip库路径
#define UNKNOWN "Unknown"
#define SHARE_MEMORY_SIZE 10485760 //必须比ip库文件大
#define INET6_ADDRSTRLEN 46
#define RECORD_LEN 7 //单条记录长度


//结果集
typedef struct 
{
    char *p_country;
    char *p_area;
    char beginip[INET6_ADDRSTRLEN]; // 用户IP所在范围的开始地址
    char endip[INET6_ADDRSTRLEN]; // 用户IP所在范围的结束地址
}location;



unsigned long getlong4(char *pos);
unsigned long getlong3(char *pos);

void createshare();
void openshare();
void closeshare();
char *getarea(char *pos);
void getipinfo(char *ipstr,location *p_loc);







#endif
