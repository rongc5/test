#include "ip_addr_info.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <mysql/mysql.h>
#include <sys/mman.h>
#include "cityidquery.h"


extern  MYSQL mysql;
extern MYSQL_RES *mysql_ret;
extern MYSQL_ROW mysql_row;

FILE *fp;
int num = 0;

int getipstrs()
{
    location locate;
    int i, num;
    char *ptr;
    char strQuery[512];
    sprintf(strQuery, "select a.mac, a.activipstr from udev_basic a inner join haier_push.temp_tvmsgid_distinct_mac b on a.mac = b.mac and a.isactiv = 1;");
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
            CIDINFO *tmp = ip_get_cityid(mysql_row[1]);
            if (tmp != NULL){
                fprintf(fp, "%-16s    %-6s    ", mysql_row[0], tmp->cityid);

                if (tmp->region_grage == PROVINCE_GRADE){
                    fprintf(fp, "                 ");
                }else if (tmp->region_grage == CITY_GRADE){
                        fprintf(fp, "%-16s    ", tmp->cname);
                }

                fprintf(fp, "%-16s    ", tmp->proname);

                memcpy(strQuery, tmp->cityid, 2);
                strQuery[2] = '\0';
                fprintf(fp, "%-6s\n", strQuery);
            }else 
                fprintf(fp, "%-16s\n", mysql_row[0]);

        }
    }

    mysql_free_result(mysql_ret);
    close_mysql();
    fclose(fp);
    return 0;
}






int main(void)
{
    fp = fopen("tmp", "w+");
    if (fp == NULL){
        perror("fopen...");
        return 1;
    }
    construct_cityid_cache();

    getipstrs();

    printf("%d\n", num);
    return 0;
}


