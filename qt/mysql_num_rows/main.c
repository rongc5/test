#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/un.h>
#include <mysql/mysql.h>
#include <pthread.h>
#include <unistd.h>
#include <netinet/in.h>

#define MYSQL_PORT      3306
#define MYSQL_SERVER    "192.168.3.115"
#define MYSQL_USER      "permitdbuser"
#define MYSQL_PWD       "permitdbpwd"
#define MYSQL_DB        "haierdb"

#define QUERY_LEN       500


int main(void)
{
    printf("--K--  ---------------111111111111111111111111-----------\n");
        MYSQL mysql;
        MYSQL_RES * mysql_ret = NULL;
        MYSQL_ROW mysql_row;
        char sqlQuery[QUERY_LEN];
        int num;
        char MACSTR[33];
        char *mac = MACSTR;
        int thread_num = 50;

        mysql_init(&mysql);
        if (mysql_real_connect(&mysql, MYSQL_SERVER, MYSQL_USER, MYSQL_PWD, MYSQL_DB, MYSQL_PORT, NULL, 0))
            printf(" Connect to the mysql success ---\n");
        else{
            printf(" Connect to the mysql failed ---\n");
            mac = NULL;
            return -1;
        }

        memset(sqlQuery, 0, QUERY_LEN);
        /*      sprintf(sqlQuery, "select mac from udev_basic order by rand() limit 1;");*/
    #if 1
        sprintf(sqlQuery, "SELECT mac FROM `udev_basic` AS t1 JOIN (SELECT ROUND(RAND() * ((SELECT MAX(udevno) FROM `udev_basic`)-(SELECT MIN(udevno) FROM `udev_basic`))+(SELECT MIN(udevno) FROM `udev_basic`)) AS udevno) AS t2 WHERE  t1.udevno>= t2.udevno ORDER BY t1.udevno LIMIT %d;", thread_num);

    #endif

        if (mysql_real_query(&mysql, sqlQuery, sizeof(sqlQuery)))
        {
            printf("%s\n", mysql_error(&mysql));
            mac = NULL;
            return -1;
        }
        mysql_ret = mysql_store_result(&mysql);
        if (NULL == mysql_ret)
        {
            printf("\n mysql error. mysql_ret is NULL. %s\n", mysql_error(&mysql));
            mac = NULL;
            return -1;
        }
        num = mysql_num_rows(mysql_ret);

     // printf ("Number of rows: %lu\n", (unsigned long) mysql_num_rows(mysql_ret));

        if(num != 50)
        {
            printf("select %d num result. not 1 \n", num);
            mac = NULL;
            return -1;
        }

         while ((mysql_row = mysql_fetch_row(mysql_ret)) != NULL)
         {
                printf("%s\n", mysql_row[0]);
         }


        mysql_free_result(mysql_ret);
        mysql_close(&mysql);
        return 0;
}

