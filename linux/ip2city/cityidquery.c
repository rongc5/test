#include "cityidquery.h"

#define PROVINCE 999 
#define CITY    888
#define TOWN 777

#define _CITY_STOP 1 //find stop at city level ?

static CIDINFO *p_share_province;
static CIDINFO *p_share_city;
static CIDINFO *p_share_town;
static ZIPSEARCH *p_zip_search;



//pthread_mutex_t m_mutex;
pthread_rwlock_t rwlock;

int province_num;
int city_num;
int town_num;


MYSQL mysql;
MYSQL_RES *mysql_ret = NULL;
MYSQL_ROW mysql_row;

static CodeTypes_t codetype;

static int mysql_port_cityid;
static char mysql_server_cityid[16];
static char mysql_user_cityid[20];
static char mysql_pwd_cityid[20];
static char mysql_db_cityid[20];
static char qqwrydat_path[256];
static char qqwrydat_type[10];
char update_time[6];

static int prse_buf_cityid(char *buf)
{
    char *p;
    int i = 0;
    if ((p = strstr(buf, "mysql_host"))){
        while (*p != '=')
            p++;
        p++;
        while (*p != '\n' && *p != '\0')
            mysql_server_cityid[i++] = *p++;
        mysql_server_cityid[i] = '\0';
        //printf("%s\n", mysql_server_cityid);
        return 0;
    }

    i = 0;
    if ((p = strstr(buf, "mysql_usr"))){
        while (*p != '=')
            p++;
        p++;
        while (*p != '\n' && *p != '\0')
            mysql_user_cityid[i++] = *p++;
        mysql_user_cityid[i] = '\0';
        //printf("%s\n", mysql_user_cityid);
        return 0;
    }


    i = 0;
    if ((p = strstr(buf, "mysql_pwd"))){
        while (*p != '=')
            p++;
        p++;
        while (*p != '\n' && *p != '\0')
            mysql_pwd_cityid[i++] = *p++;
        mysql_pwd_cityid[i] = '\0';
        //printf("%s\n", mysql_pwd_cityid);
        return 0;
    }

    i = 0;
    if ((p = strstr(buf, "mysql_database"))){
        while (*p != '=')
            p++;
        p++;
        while (*p != '\n' && *p != '\0')
            mysql_db_cityid[i++] = *p++;
        mysql_db_cityid[i] = '\0';
        //printf("%s\n", mysql_db_cityid);
        return 0;
    }


    i = 0;
    if ((p = strstr(buf, "mysql_port"))){
        while (*p != '=')
            p++;
        p++;
        while (*p != '\n' && *p != '\0')
            buf[i++] = *p++;
        buf[i] = '\0';
        mysql_port_cityid = atoi(buf);
        //printf("%d\n", mysql_port_cityid);
        return 0;
    }

    if ((p = strstr(buf, "qqwrydat_path"))){
        while (*p != '=')
            p++;
        p++;
        while (*p != '\n' && *p != '\0')
            qqwrydat_path[i++] = *p++;
        qqwrydat_path[i] = '\0';
        //printf("%s\n", qqwrydat_path);
        return 0;
    }

    if ((p = strstr(buf, "qqwrydat_type"))){
        while (*p != '=')
            p++;
        p++;
        while (*p != '\n' && *p != '\0')
            qqwrydat_type[i++] = *p++;
        qqwrydat_type[i] = '\0';
        //printf("%s\n", qqwrydat_type);
        return 0;
    }

    if ((p = strstr(buf, "update_time"))){
        while (*p != '=')
            p++;
        p++;
        while (*p != '\n' && *p != '\0')
            update_time[i++] = *p++;
        update_time[i] = '\0';
        //printf("%s\n", update_time);
        return 0;
    }
}


static int init_from_file(char *v)
{
    char buf[512];
    char *retptr;
    int i = 0;
    FILE * fp = fopen(v, "r");
    if (fp == NULL){
        perror("fopen...");
        exit(1);
    }

    while (1){
        retptr = fgets(buf, 512, fp);
        if (NULL == retptr) {
            if (feof(fp))
                break;
            else {
                perror("fgets...");
                continue;
            }
        }

        if (!strstr(buf, "#")){
            prse_buf_cityid(buf);
        }
    }


    if (strstr(qqwrydat_type, "UTF8") || strstr(qqwrydat_type, "utf8") || strstr(qqwrydat_type, "UTF-8") || strstr(qqwrydat_type, "utf-8"))
        codetype = UTF_t;
    else 
        codetype = GBK_t;

    return 0;
}

static int init_mysql_cityid()
{
    char strQuery[512];
    mysql_init(&mysql);
    if (mysql_real_connect(&mysql, mysql_server_cityid, mysql_user_cityid, mysql_pwd_cityid, 
                mysql_db_cityid, mysql_port_cityid, NULL, 0))
        printf("Connect to the mysql success...\n");
    else {
        printf("%s\n", mysql_error(&mysql));
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

static int close_mysql_cityid()
{
    mysql_close(&mysql);
}

static int get_regions_num()
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

int compar(const void *a, const void *b)
{
    ZIPSEARCH * m = (ZIPSEARCH *)a;
    ZIPSEARCH * n = (ZIPSEARCH *)b;

    if (m->zipt > n->zipt)
        return 1;
    else if (m->zipt < n->zipt)
        return -1;
    else
        return 0;
}


void sort_zipd_search()
{
    ZIPSEARCH * ptmp = p_zip_search;
    qsort(ptmp, city_num, sizeof(ZIPSEARCH), compar);

    return ;
}

void list_show_zip()
{
	int i = 0;
	ZIPSEARCH * ptmp = p_zip_search;
	for (i; i < city_num; i++, ptmp++){
			printf("%-6d	%-6d", ptmp->zipc, ptmp->zipt);
			if (!i%3)
				printf("\n");
	}
	printf("\n");
	return ;
}

int binArySearch_t(ZIPSEARCH a[], int num, int tmp)
{
    int high, low, mid;
    low = 0, high = num - 1;
    if (tmp > a[high].zipt || tmp < a[low].zipt){
        //printf("I can't find the num %d\n", tmp);
        return -1;
    }

    while (low <= high){
        mid = (high + low)/2;
        if (a[mid].zipt > tmp)
            high = mid - 1;
        else if (a[mid].zipt < tmp)
            low = mid + 1;
        else if (a[mid].zipt == tmp){
            //printf("I find it a[%d]=%d\n", mid, a[mid]);
            return mid;
        }
    }
    //printf("I don't find it");
    return -1;
}


int binArySearch_c(ZIPSEARCH a[], int num, int tmp)
{
    int high, low, mid;
    low = 0, high = num - 1;
    if (tmp > a[high].zipc || tmp < a[low].zipc){
        //printf("I can't find the num %d\n", tmp);
        return -1;
    }

    while (low <= high){
        mid = (high + low)/2;
        if (a[mid].zipc > tmp)
            high = mid - 1;
        else if (a[mid].zipc < tmp)
            low = mid + 1;
        else if (a[mid].zipc == tmp){
            //printf("I find it a[%d]=%d\n", mid, a[mid]);
            return mid;
        }
    }
    //printf("I don't find it");
    return -1;
}


static int init_province_cache()
{
    int i, num;
    char strQuery[512];
    CIDINFO *pos = p_share_province;
    sprintf(strQuery, "SELECT proid, proname, countryname, zipcode from `ssc_province`;");
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
            memcpy(pos->townid, "0", sizeof("0"));
            memcpy(pos->tname, "0", sizeof("0"));
            memcpy(pos->cityid, "0", sizeof("0"));
            memcpy(pos->cname, "0", sizeof("0"));
            memcpy(pos->provinceid, mysql_row[0], strlen(mysql_row[0]));
            memcpy(pos->proname, mysql_row[1], strlen(mysql_row[1]));
            memcpy(pos->countryname, mysql_row[2], strlen(mysql_row[2]));
            memcpy(pos->zipcode, mysql_row[3], strlen(mysql_row[3]));
            pos->region_grage = PROVINCE_GRADE;
            pos++;
        }
    }

    mysql_free_result(mysql_ret);
    return 0;
}




static int init_city_cache()
{
    int i, num, ret, tmpm;
    char strQuery[512];
    memset(strQuery, 0, 512);
    CIDINFO *tmp = p_share_province;
    CIDINFO *pos = p_share_city;
    ZIPSEARCH *pzip_tmp = p_zip_search;

    for (i = 0; i < province_num; i++){
        if (strlen(tmp->provinceid) >= 6)
            sprintf(strQuery, "SELECT cid, cname, zipcode from `ssc_city` where proid = %s;", tmp->provinceid);
        else
            continue;
        if (mysql_real_query(&mysql, strQuery, sizeof(strQuery))){
            printf("22222%s\n", mysql_error(&mysql));
            return -1;
        }

        mysql_ret = mysql_store_result(&mysql);
        if (NULL == mysql_ret){
        	  printf("%s\n", __func__);
            printf("mysql error. mysql_ret is NULL. %s\n", mysql_error(&mysql));
            return -1;
        }

        num = mysql_num_rows(mysql_ret);


        for (i = 0; i < num; i++){
            if ((mysql_row = mysql_fetch_row(mysql_ret)) != NULL){
                memcpy(pos->townid, "0", 2);
                memcpy(pos->tname, "0", 2);
                memcpy(pos->cityid, mysql_row[0], strlen(mysql_row[0]));
                memcpy(pos->cname, mysql_row[1], strlen(mysql_row[1]));
                memcpy(pos->zipcode, mysql_row[2], strlen(mysql_row[2]));
                memcpy(pos->provinceid, tmp->provinceid, strlen(tmp->provinceid));
                memcpy(pos->proname, tmp->proname, strlen(tmp->proname));
                memcpy(pos->countryname, tmp->countryname, strlen(tmp->countryname));
                pos->region_grage = CITY_GRADE;
                
                pzip_tmp->pCidInfo = pos;
                tmpm = atoi(pos->zipcode);
                ret = tmpm/100;
                pzip_tmp->zipt = ret*100;
                ret = tmpm/1000;
                pzip_tmp->zipc =ret*1000;
                
                //printf("%s	%d\n", pos->zipcode, p_zip_search[i].zipd);
                
                if (i == 0){
                    tmp->next_level_start = pos;
                }
                pzip_tmp++;
                pos++;
            }
        }

        tmp->next_level_stop = pos;

        tmp++;
        mysql_free_result(mysql_ret);
    }

    return 0;
}

static int init_town_cache()
{
    int i, num, n;
    char strQuery[512];
    CIDINFO *tmp = p_share_city;
    CIDINFO *pos = p_share_town;

    for (i = 0; i < city_num; i++){
        if (strlen(tmp->cityid) >= 6)
            sprintf(strQuery, "SELECT tid, tname, cid, zipcode from `ssc_town` where cid = %s", tmp->cityid);
        else
            continue;
        if (mysql_real_query(&mysql, strQuery, sizeof(strQuery))){
        	  //printf("%s\n", __func__);
            return -1;
        }
			
        mysql_ret = mysql_store_result(&mysql);
        if (NULL == mysql_ret){
        	printf("%s\n", __func__);
        		//printf("%s\n", strQuery);
            //printf("mysql error. mysql_ret is NULL. %s\n", mysql_error(&mysql));
            return -1;
        }

        num = mysql_num_rows(mysql_ret);

        for (i = 0; i < num; i++){
            if ((mysql_row = mysql_fetch_row(mysql_ret)) != NULL){
                memcpy(pos->townid, mysql_row[0], strlen(mysql_row[0]));
                memcpy(pos->tname, mysql_row[1], strlen(mysql_row[1]));
                if (mysql_row[3])
                memcpy(pos->zipcode, mysql_row[3], strlen(mysql_row[3]));
                else
                memcpy(pos->zipcode, "000000", sizeof("000000"));
                memcpy(pos->provinceid, tmp->provinceid, strlen(tmp->provinceid));
                memcpy(pos->cityid, tmp->cityid, strlen(tmp->cityid));
                memcpy(pos->proname, tmp->proname, strlen(tmp->proname));
                memcpy(pos->cname, tmp->cname, strlen(tmp->cname));
                memcpy(pos->countryname, tmp->countryname, strlen(tmp->countryname));
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



static int create_share_mem(void)
{
    p_share_province = (CIDINFO *)mmap(NULL, sizeof(CIDINFO)*province_num, PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (p_share_province == MAP_FAILED){
        //printf("%s\n", strerror(errno));
        exit(1);
    }

    memset(p_share_province, 0, sizeof(CIDINFO)*province_num);
    p_share_city = (CIDINFO *)mmap(NULL, sizeof(CIDINFO)*city_num, PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (p_share_city == MAP_FAILED){
        //printf("%s\n", strerror(errno));
        exit(1);
    }

    memset(p_share_city, 0, sizeof(CIDINFO)*city_num);
    p_share_town = (CIDINFO *)mmap(NULL, sizeof(CIDINFO)*town_num, PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (p_share_town == MAP_FAILED){
        //printf("%s\n", strerror(errno));
        exit(1);
    }
    memset(p_share_town, 0, sizeof(CIDINFO)*town_num);

    p_zip_search = (ZIPSEARCH *)mmap(NULL, sizeof(ZIPSEARCH)*city_num, PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (p_zip_search == MAP_FAILED){
        //printf("%s\n", strerror(errno));
        exit(1);
    }
    memset(p_zip_search, 0, sizeof(ZIPSEARCH)*city_num);

    return 0;
}

int my_iconv(char *charset_to, char *charset_from, char *buff_out, size_t len_out, char *buff_in, size_t len_in)  
{  
    int n = len_out;  
    iconv_t convd;  

    convd = iconv_open (charset_to, charset_from);   
    if (convd == (iconv_t)(-1))  
    {  
        return 1;  
    }  
    iconv (convd, &buff_in, &len_in, &buff_out, &len_out);  
    iconv_close(convd);  

    return 0;  
} 

int zipcode_get_cityid(char *zipcode, CIDINFO * pcid)
{
    ZIPSEARCH *pos = p_zip_search;
    int ret, zip, tmp;
    memset(pcid, 0, sizeof(CIDINFO));


    tmp = atoi(zipcode);
   // printf("%s	zipcode:%d\n", __func__, tmp);
    ret = tmp/100;
    zip = ret * 100;
   // printf("%s	zipd:%d\n", __func__, zip);

    ret = binArySearch_t(pos, city_num, zip);
    if (ret != -1){
			goto out;
    }else{
		    ret = tmp/1000;
		    zip = ret * 1000;
		    ret = binArySearch_c(pos, city_num, zip);
		    if (ret == -1)
		    	return -1;
		    else
                goto out;
    }
        
out:
    memcpy(pcid, pos[ret].pCidInfo, sizeof(CIDINFO));

    return 0;
}

int ip_get_cityid(char *ipstr, ALLINFO * pall)
{		
    //pthread_mutex_lock(&m_mutex);
    CIDINFO *pos = p_share_province;
    CIDINFO *tmp = NULL, *tmp1 = NULL, *tmp2 = NULL;
    CIDINFO *end;
    int i, num, ret;
    memset(pall, 0, sizeof(ALLINFO));
    localtmp lotmp;


    pthread_rwlock_rdlock(&rwlock);

    if (getipinfo(ipstr, &lotmp)){
        pthread_rwlock_unlock(&rwlock);
        return -1;
    }


    if (codetype == GBK_t){
        ret = my_iconv((char*)"utf8", (char*)"gbk", pall->locInfo.countrybuf, sizeof(pall->locInfo.countrybuf), lotmp.p_country, strlen(lotmp.p_country)); 
        if (ret == 1){
            //	pthread_mutex_unlock(&m_mutex);
            pthread_rwlock_unlock(&rwlock);
            return -1;
        }
        ret = my_iconv((char*)"utf8", (char*)"gbk", pall->locInfo.areabuf, sizeof(pall->locInfo.areabuf), lotmp.p_area, strlen(lotmp.p_area)); 
        if (ret == 1){
            //pthread_mutex_unlock(&m_mutex);
            pthread_rwlock_unlock(&rwlock);
            return -1;
        }
    }else if (codetype == UTF_t){
        memcpy(pall->locInfo.countrybuf, lotmp.p_country, strlen(lotmp.p_country));
        memcpy(pall->locInfo.areabuf, lotmp.p_area, strlen(lotmp.p_area));
    }

    memcpy(pall->locInfo.beginip, lotmp.beginip, strlen(lotmp.beginip));
    memcpy(pall->locInfo.endip, lotmp.endip, strlen(lotmp.endip));

    pthread_rwlock_unlock(&rwlock);

    printf("%s  %s\n", pall->locInfo.countrybuf, pall->locInfo.areabuf);

    for (i = 0; i < province_num; i++){
        if (strstr(pall->locInfo.countrybuf, pos->proname)){
            tmp = pos;
            break;
        }
        pos++;
    }

    if (tmp == NULL){
        //pthread_mutex_unlock(&m_mutex);
        return -1;
    }


    pos = tmp->next_level_start;
    end = tmp->next_level_stop;
    while (pos != end){
        if (strstr(pall->locInfo.countrybuf, pos->cname) && strstr(pall->locInfo.countrybuf, pos->proname)){
            tmp1 = pos;
            break;
        }
        pos++;
    }

    if (tmp1 == NULL){
        memcpy(&pall->cityidInfo, tmp, sizeof(CIDINFO));
        //pthread_mutex_unlock(&m_mutex);
        goto out;
    }

#if _CITY_STOP
    memcpy(&pall->cityidInfo, tmp1, sizeof(CIDINFO));
    //printf("pall->cityidInfo.townid: %s\n", &pall->cityidInfo.townid);
    goto out;
#endif

    pos = tmp1->next_level_start;
    end = tmp1->next_level_stop;
    while (pos != end){
        if (strstr(pall->locInfo.countrybuf, pos->tname) && strstr(pall->locInfo.countrybuf, pos->cname) && strstr(pall->locInfo.countrybuf, pos->proname)){
            tmp2 = pos;
            break;
        }
        pos++;
    }

    if (tmp2 == NULL)
        memcpy(&pall->cityidInfo, tmp1, sizeof(CIDINFO));
    else 
        memcpy(&pall->cityidInfo, tmp2, sizeof(CIDINFO));
    //pthread_mutex_unlock(&m_mutex);


out:
    return 0;
}


int construct_cityid_cache(char *v)
{
    //pthread_mutex_init(&m_mutex, NULL);
    pthread_rwlock_init(&rwlock, NULL);
    init_from_file(v);
    init_mysql_cityid();

    get_regions_num();
    create_share_mem();
    init_province_cache();
    init_city_cache();
    init_town_cache();
    

    sort_zipd_search();
    //openshare(qqwrydat_path);

    close_mysql_cityid();
    if (createThread(qqwrydat_path) < 0)
        return -1;
    return 0;
}

void destroy_cityid_cache()
{
    closeshare(); // ÊÍ·ÅÄÚ´æ
    if (p_share_province)
    {
        printf("===>close p_share_province\n");
        munmap( p_share_province, sizeof(CIDINFO)*province_num);
    }

    if (p_share_city)
    {
        printf("===>close p_share_city\n");
        munmap( p_share_city, sizeof(CIDINFO)*city_num);
    }

    if (p_share_town)
    {
        printf("===>close p_share_town\n");
        munmap( p_share_town, sizeof(CIDINFO)*town_num);
    }

    //pthread_mutex_destroy(&m_mutex);
    pthread_rwlock_destroy(&rwlock);
}




