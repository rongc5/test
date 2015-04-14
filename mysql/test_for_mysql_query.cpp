#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include <errno.h>

#define MYSQL_PORT      3307
#define MYSQL_SERVER    "119.57.11.34"
#define MYSQL_USER      "permitdbuser"
#define MYSQL_PWD       "permitdbpwd"
#define MYSQL_DB        "haierdb"


int main(void)
{
    MYSQL *m_sql;
    MYSQL_RES *res;
    MYSQL_ROW rows;
    MYSQL_FIELD *field;
    MYSQL_FIELD *fields;
    int i = 0;
    int num = 0, row = 0;
    int m_query=0;
    char command[120] = "";

    m_sql = (MYSQL*)malloc(sizeof(MYSQL));

    mysql_init(m_sql);

    mysql_options(m_sql, MYSQL_READ_DEFAULT_GROUP, "mysql");

    if (!mysql_real_connect(m_sql, MYSQL_SERVER, MYSQL_USER, MYSQL_PWD, MYSQL_DB, MYSQL_PORT, NULL, 0))
    {
        printf("step1\n");
        perror("mysql_real_connect!");
        exit(1);
    }

    printf("Connect MySQL Success!\n");

    //m_query = mysql_query(m_sql, "insert ignore into t_Upass_DevSid_Update(udevno) value(000001);");
    m_query = mysql_query(m_sql, "select min(epg_date) from statistics.TOP_TVRating where epg_date is not null;");
    if (m_query)
    {
        printf("step3 m_query= 	%d\n", m_query);
        perror("mysql_query");
      //  exit(1);
    }

    printf("m_query:	%d\n", m_query);
    mysql_free_result(res);

    free(m_sql);
//    mysql_close(m_sql);

    return 0;
}

