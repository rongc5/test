#ifndef __CITYIDQUERY_H_
#define __CITYIDQUERY_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <mysql/mysql.h>
#include <sys/mman.h>

#include <fcntl.h>
#include <sys/types.h>
#include <math.h>

#include <sys/stat.h>
#include <netinet/in.h>
#include <errno.h>

#include <iconv.h>
#include <pthread.h>


#define UNKNOWN "unknow"
#define INET6_ADDRSTRLEN 46
#define RECORD_LEN 7 //单条记录长度


//结果集
typedef struct 
{
    char countrybuf[32];
    char areabuf[32];
    char beginip[INET6_ADDRSTRLEN]; // 用户IP所在范围的开始地址
    char endip[INET6_ADDRSTRLEN]; // 用户IP所在范围的结束地址
}location;

typedef struct 
{
    char *p_country;
    char *p_area;
    char beginip[INET6_ADDRSTRLEN]; // 用户IP所在范围的开始地址
    char endip[INET6_ADDRSTRLEN]; // 用户IP所在范围的结束地址
}localtmp;


unsigned int getlong4(char *pos);
unsigned int getlong3(char *pos);

void createshare(char *qqwrydat_path);
int openshare(char *qqwrydat_path);
void closeshare();
char *getarea(char *pos);
int getipinfo(char *ipstr,localtmp *p_loc);

void *updateQqwry(void *pPtr);
int createThread(char *qqwrydat_path);

typedef enum {
    UTF_t = 1,
    GBK_t
}CodeTypes_t;


typedef enum {
    COUNTRY_GRADE = 1,
    PROVINCE_GRADE,
    CITY_GRADE,
    TOWN_GRADE
}Grades_t;


typedef struct CidInfomation{
    Grades_t region_grage;
    char zipcode[12];
    char townid[12];
    char cityid[12];
    char provinceid[12];
    char countryname[20];
    char proname[32];
    char cname[32];
    char tname[32];
    struct CidInfomation *next_level_start;
    struct CidInfomation *next_level_stop;
}CIDINFO;

typedef struct AllInfomation{
    CIDINFO cityidInfo;
    location locInfo;
}ALLINFO;

typedef struct ZipSearch{
    int zipc;
    int zipt;
    CIDINFO * pCidInfo;
}ZIPSEARCH;

static int init_from_file(char *v);
int construct_cityid_cache(char *v);
void destroy_cityid_cache();
int ip_get_cityid(char *ipstr, ALLINFO * pall);
int zipcode_get_cityid(char *zipcode, CIDINFO * pcid);
int binArySearch_c(ZIPSEARCH a[], int num, int tmp);
int binArySearch_t(ZIPSEARCH a[], int num, int tmp);
void sort_zipd_search();
int compar(const void *a, const void *b);
void list_show_zip();


static int create_share_mem(void);
static int init_town_cache();
static int init_city_cache();
static int init_province_cache();
static int get_regions_num();
static int close_mysql_cityid();


static int init_mysql_cityid();
static int prse_buf_cityid(char *buf);











#endif
