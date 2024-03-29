#include "cityidquery.h"
#include "ip_addr_info.h"

#define PROVINCE 999 
#define CITY    888
#define TOWN 777


static CIDINFO *p_share_province;
static CIDINFO *p_share_city;
static CIDINFO *p_share_town;


static int province_num;
static int city_num;
static int town_num;

extern int num;

MYSQL mysql;
MYSQL_RES *mysql_ret = NULL;
MYSQL_ROW mysql_row;

#define MYSQL_PORT  3306 
#define MYSQL_SERVER "122.70.151.106"
#define MYSQL_USER "permitdbuser"
#define MYSQL_PWD "permitdbpwd"
#define MYSQL_DB "haierdb"


int init_mysql()
{
    char strQuery[512];
    mysql_init(&mysql);
    if (mysql_real_connect(&mysql, MYSQL_SERVER, MYSQL_USER, MYSQL_PWD, 
                MYSQL_DB, MYSQL_PORT, NULL, 0))
        printf("Connect to the mysql success...\n");
    else {
        printf("Connect to the mysql failed...\n");
        return -1;
    }
    sprintf(strQuery, "set names utf8;");
    if (mysql_real_query(&mysql, strQuery, sizeof(strQuery))){
        printf("%s\n", mysql_error(&mysql));
        return -1;
    }

    mysql_free_result(mysql_ret);
    return 0;
}

int close_mysql()
{
    mysql_close(&mysql);
}

int get_regions_num()
{
    char strQuery[512];
    int num;

    sprintf(strQuery, "select count(1) from `ssc_province`;");
    if (mysql_real_query(&mysql, strQuery, sizeof(strQuery))){
        printf("%s\n", mysql_error(&mysql));
        return -1;
    }
    
    mysql_ret = mysql_store_result(&mysql);
    if (NULL == mysql_ret){
        printf("mysql error. mysql_ret is NULL. %s\n", mysql_error(&mysql));
        return -1;
    }
    
    num = mysql_num_rows(mysql_ret);
    while ((mysql_row = mysql_fetch_row(mysql_ret)) != NULL){
        province_num = atoi(mysql_row[0]);
    }

    mysql_free_result(mysql_ret);

    sprintf(strQuery, "SELECT count(1) from `ssc_city`;");
    if (mysql_real_query(&mysql, strQuery, sizeof(strQuery))){
        printf("%s\n", mysql_error(&mysql));
        return -1;
    }
    
    mysql_ret = mysql_store_result(&mysql);
    if (NULL == mysql_ret){
        printf("mysql error. mysql_ret is NULL. %s\n", mysql_error(&mysql));
        return -1;
    }
    
    num = mysql_num_rows(mysql_ret);
    while ((mysql_row = mysql_fetch_row(mysql_ret)) != NULL){
        city_num = atoi(mysql_row[0]);
    }
    mysql_free_result(mysql_ret);

    sprintf(strQuery, "SELECT count(1) from `ssc_town`;");
    if (mysql_real_query(&mysql, strQuery, sizeof(strQuery))){
        printf("%s\n", mysql_error(&mysql));
        return -1;
    }
    
    mysql_ret = mysql_store_result(&mysql);
    if (NULL == mysql_ret){
        printf("mysql error. mysql_ret is NULL. %s\n", mysql_error(&mysql));
        return -1;
    }
    
    num = mysql_num_rows(mysql_ret);
    while ((mysql_row = mysql_fetch_row(mysql_ret)) != NULL){
        town_num = atoi(mysql_row[0]);
    }
    mysql_free_result(mysql_ret);

	//printf(" province_num:%d, city_num:%d, town_num:%d\n", province_num, city_num, town_num);

    return 0;
}

int init_province_cache()
{
    int i, num;
    char strQuery[512];
    CIDINFO *pos = p_share_province;
    sprintf(strQuery, "SELECT proid, proname, countryname from `ssc_province`;");
    if (mysql_real_query(&mysql, strQuery, sizeof(strQuery))){
        printf("%s\n", mysql_error(&mysql));
        return -1;
    }
    
    mysql_ret = mysql_store_result(&mysql);
    if (NULL == mysql_ret){
        printf("mysql error. mysql_ret is NULL. %s\n", mysql_error(&mysql));
        return -1;
    }
    
    num = mysql_num_rows(mysql_ret);
    

    for (i = 0; i < num; i++){
        if ((mysql_row = mysql_fetch_row(mysql_ret)) != NULL){
            memcpy(pos->cityid, mysql_row[0], strlen(mysql_row[0]));
            memcpy(pos->proname, mysql_row[1], strlen(mysql_row[1]));
            memcpy(pos->countryname, mysql_row[2], strlen(mysql_row[2]));
            pos->region_grage = PROVINCE_GRADE;
            //printf("proname:%s\n", pos->cityid);
            pos++;
        }
    }
    
    mysql_free_result(mysql_ret);
    return 0;
}

int init_city_cache()
{
    int i, num;
    char strQuery[512];
    memset(strQuery, 0, 512);
    CIDINFO *tmp = p_share_province;
    CIDINFO *pos = p_share_city;

    for (i = 0; i < province_num; i++){
        if (strlen(tmp->cityid) >= 6)
            sprintf(strQuery, "SELECT cid, cname from `ssc_city` where proid = %s;", tmp->cityid);
        else
            continue;
        if (mysql_real_query(&mysql, strQuery, sizeof(strQuery))){
            printf("22222%s\n", mysql_error(&mysql));
            return -1;
        }

        mysql_ret = mysql_store_result(&mysql);
        if (NULL == mysql_ret){
            printf("mysql error. mysql_ret is NULL. %s\n", mysql_error(&mysql));
            return -1;
        }

        num = mysql_num_rows(mysql_ret);


        for (i = 0; i < num; i++){
            if ((mysql_row = mysql_fetch_row(mysql_ret)) != NULL){
                memcpy(pos->cityid, mysql_row[0], 8);
                memcpy(pos->cname, mysql_row[1], 32);
                memcpy(pos->proname, tmp->proname, 32);
                memcpy(pos->countryname, tmp->countryname, 20);
                pos->region_grage = CITY_GRADE;
                if (i == 0){
                    tmp->next_level_start = pos;
                }
                pos++;
            }
        }

    tmp->next_level_stop = pos;
    
    tmp++;
    mysql_free_result(mysql_ret);
    }

    return 0;
}

int init_town_cache()
{
    int i, num, n;
    char strQuery[512];
    CIDINFO *tmp = p_share_city;
    CIDINFO *pos = p_share_town;
    
    for (i = 0; i < city_num; i++){
        if (strlen(tmp->cityid) >= 6)
        sprintf(strQuery, "SELECT tid, tname from `ssc_town` where cid = %s;", tmp->cityid);
        else
            continue;
        if (mysql_real_query(&mysql, strQuery, sizeof(strQuery))){
            printf("1111111%s\n", mysql_error(&mysql));
            return -1;
        }

        mysql_ret = mysql_store_result(&mysql);
        if (NULL == mysql_ret){
            printf("mysql error. mysql_ret is NULL. %s\n", mysql_error(&mysql));
            return -1;
        }

        num = mysql_num_rows(mysql_ret);

        for (i = 0; i < num; i++){
            if ((mysql_row = mysql_fetch_row(mysql_ret)) != NULL){
                memcpy(pos->cityid, mysql_row[0], strlen(mysql_row[0]) + 1);
                memcpy(pos->tname, mysql_row[1], strlen(mysql_row[1]) + 1);
                memcpy(pos->proname, tmp->proname, 32);
                memcpy(pos->cname, tmp->cname, 32);
                memcpy(pos->countryname, tmp->cname, 20);
                pos->region_grage = TOWN_GRADE;
                if (i == 0){
                    tmp->next_level_start = pos;
                }
                pos++;
            }
        }

    tmp->next_level_stop = pos;
    
    tmp++;
    mysql_free_result(mysql_ret);
    }
	
    return 0;
}



int create_share_mem(void)
{
	p_share_province = (CIDINFO *)mmap(NULL, sizeof(CIDINFO)*province_num, PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	if (p_share_province == MAP_FAILED){
        printf("%s\n", strerror(errno));
        exit(1);
    }

    memset(p_share_province, 0, sizeof(CIDINFO)*province_num);
	p_share_city = (CIDINFO *)mmap(NULL, sizeof(CIDINFO)*city_num, PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	if (p_share_city == MAP_FAILED){
        printf("%s\n", strerror(errno));
        exit(1);
    }

    memset(p_share_city, 0, sizeof(CIDINFO)*city_num);
	p_share_town = (CIDINFO *)mmap(NULL, sizeof(CIDINFO)*town_num, PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	if (p_share_town == MAP_FAILED){
        printf("%s\n", strerror(errno));
        exit(1);
    }
    memset(p_share_town, 0, sizeof(CIDINFO)*town_num);
    
    return 0;
}


CIDINFO * ip_get_cityid(char *ipstr)
{
    CIDINFO *pos = p_share_province;
    CIDINFO *tmp = NULL, *tmp1 = NULL;
    CIDINFO *end;
    int i, num;

    location locate;
    getipinfo(ipstr, &locate);

    for (i = 0; i < province_num; i++){
        if (strstr(locate.p_country, pos->proname)){
            tmp = pos;
            break;
        }
        pos++;
    }

    if (tmp == NULL){
        return NULL;
    }


    pos = tmp->next_level_start;
    end = tmp->next_level_stop;
    while (pos != end){
        if (strstr(locate.p_country, pos->cname) && strstr(locate.p_country, pos->proname)){
            tmp1 = pos;
	    num++;
            break;
        }
        pos++;
    }

    if (tmp1 == NULL)
        return tmp;

#ifdef CITY_D
    return tmp1;
#endif
    tmp = NULL;
    pos = tmp1->next_level_start;
    end = tmp1->next_level_stop;
    while (pos != end){
        if (strstr(locate.p_country, pos->tname) && strstr(locate.p_country, pos->cname) && strstr(locate.p_country, pos->proname)){
            tmp = pos;
            break;
        }
        pos++;
    }

    if (tmp == NULL)
        return tmp1;

    return tmp;;
}



int truncate_and_save_string(const char *start, char *stop, char *save)
{
    if (start == NULL || stop == NULL || save == NULL)
        return 1;

    while (*start != *stop)
        *save++ = *start++;

    *save = '\0';

    return 0;
}


int construct_cityid_cache(void)
{
    init_mysql();

    get_regions_num();
    create_share_mem();
    init_province_cache();
    init_city_cache();
    init_town_cache();

    openshare();
    
    return 0;
}




