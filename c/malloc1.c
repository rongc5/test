#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct{
    int stu;
    int number;
    char buf[33];
}DATA;

int main(void)
{
    char *p = (char *)malloc(sizeof(DATA));

    DATA *pdata = (DATA *)p;
    pdata->stu = 100;
    pdata->number = 200;
    memcpy(pdata->buf, "qianfeng", 13);

    printf("%s\n", pdata->buf);
    printf("%d\n", pdata->number);

    memset(pdata, 0, sizeof(*pdata));

    
    printf("%s\n", pdata->buf);
    printf("%d\n", pdata->number);
    return 0;
}
