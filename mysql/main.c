#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mysql/mysql.h>

char *mhost="119.57.11.34";
char *musr="permitdbuser";
char *mpwd="permitdbpwd";
int mport=3307;
char *mdatabase="haierdb";


MYSQL sql;

void initmysql(MYSQL * mysql)
{
    char strQuery[512];
    int value = 1;
    mysql_init(mysql);
    if (mysql_real_connect(mysql, mhost, musr, mpwd, mdatabase, mport, NULL, 0))
        printf(" Connect to the mysql success ---\n");
    else{
        printf(" Connect to the mysql failed ---\n");
        return ;
    }
    sprintf(strQuery, "set names utf8;");

    mysql_real_query(mysql, strQuery, sizeof(strQuery));

    mysql_options(mysql, MYSQL_OPT_RECONNECT, (char *)&value);
}


int getipstrs()
{	
    MYSQL_RES * mysql_ret = NULL;
    MYSQL_ROW mysql_row;


    int i, num, ret, value = 1;
    char strQuery[512];

    initmysql(&sql);
    //sprintf(strQuery, "select activipstr, udevno from udev_basic where isactiv = 1 and LENGTH(provincecode) < 6;");
    //sprintf(strQuery, "select activipstr, udevno, provincecode from haierdb.udev_basic where isactiv = 1;");
    //sprintf(strQuery, "select min(epg_date) from statistics.TOP_TVRating where epg_date is not null;");
    sprintf(strQuery, "select min(epg_date) from haierdb.epg_program;");
    if (mysql_real_query(&sql, strQuery, sizeof(strQuery))){
        printf("%s\n", mysql_error(&sql));
        return -1;
    }

    mysql_ret = mysql_store_result(&sql);
    if (NULL == mysql_ret){
        printf("mysql error. mysql_ret is NULL. %s\n", mysql_error(&sql));
        return -1;
    }

    if (mysql_ret){
        MYSQL_FIELD *f = mysql_fetch_field(mysql_ret);
        while (f){
            f = mysql_fetch_field(mysql_ret);
        }
    }

    num = mysql_num_rows(mysql_ret);
    printf("num:%d\n", num);
    //printf("date:%s\n", *mysql_row);
    mysql_row = mysql_fetch_row(mysql_ret);
    printf("%s\n", *mysql_row);
#if 0
    for (i = 0; i < num; i++){
        if ((mysql_row = mysql_fetch_row(mysql_ret))){
            if (strlen(mysql_row[2]) == 6)
                continue;
            if (mysql_row[0]){
                struct node *e = (struct node *)malloc(sizeof(struct node));
                if (e == NULL){
                    perror("malloc");
                    exit(0);
                }
                memset(e, 0, sizeof(struct node));
                memcpy(e->da.activipstr, mysql_row[0], strlen(mysql_row[0]));
                memcpy(e->da.udevno, mysql_row[1], strlen(mysql_row[1]));
                list_add_head(&g_node.list_entry, &(e->list_entry));
                //printf("%s  %s\n", e->da.activipstr, e->da.udevno);
            }
        }
    }
#endif
    mysql_free_result(mysql_ret);

    return 0;
}

int main(int c, char **v)
{	

    getipstrs();

    return 0;
}
