#ifndef __CITYIDQUERY_H_
#define __CITYIDQUERY_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <mysql/mysql.h>
#include <sys/mman.h>


typedef enum {
    COUNTRY_GRADE = 1,
    PROVINCE_GRADE,
    CITY_GRADE,
    TOWN_GRADE
}Grades_t;


typedef struct CidInfomation{
    Grades_t region_grage;
    char cityid[8];
    char countryname[20];
    char proname[32];
    char cname[32];
    char tname[32];
    struct CidInfomation *next_level_start;
    struct CidInfomation *next_level_stop;
}CIDINFO;


int construct_cityid_cache(void);
int truncate_and_save_string(const char *start, char *stop, char *save);
CIDINFO * ip_get_cityid(char *ipstr);
int create_share_mem(void);
int init_town_cache();
int init_city_cache();
int init_province_cache();
int get_regions_num();
int close_mysql();


int init_mysql();











#endif
