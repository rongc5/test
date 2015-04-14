#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *ptr = "hello";

typedef struct{
    char buf[20];
}date;


void v_print(char *p)
{
    printf("%d\n", sizeof(p));
}



int main(void)
{
    date *pdate = (date *)malloc(sizeof(date));
    /*printf("%d, %d, %d\n", sizeof("1"), sizeof(ptr), sizeof(pdate->buf));*/
	/*printf("%d\n", sizeof(char[1]));    */

   // v_print(ptr);
    return 0;
}
