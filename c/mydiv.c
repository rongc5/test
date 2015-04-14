#include <stdio.h>
#include <string.h>

int fun(long x, long y, int n, char buf[])
{
    long int u;

    printf("结果：\n");
    if (!buf){
        return -1;
    }

    sprintf(buf +  strlen(buf), "%d.", x/y);

    while(n--)
   {
    u=x%y;
    x=u*10;
    sprintf(buf + strlen(buf), "%d",x/y);
    if(u==0)
     break;
   }

   return 0;
}

int main(int c, char *v[])
{
    if (c < 3){
        printf("usage...\n");
        return 1;
    }

    char buf[32];
    fun(atoi(v[1]), atoi(v[2]), 8, buf);
    printf("%s\n", buf);

    return 0;
}
