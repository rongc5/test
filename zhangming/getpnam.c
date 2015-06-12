#include <shadow.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>

int main(int c, char **v)
{
    if (c != 2)
    {
        fprintf(stderr, "usage...\n");
        return -1;
    }

    struct spwd * res;
    char buf[20];
    char * pass;

    res = getspnam(v[1]);
    if (res == NULL)
    {
        perror("getspnam");
        exit(1);
    }
l1:
    printf("please input your password:\n");
    scanf("%s", buf);
    pass = (char *)crypt(buf, res->sp_pwdp);
    if (NULL == pass)
    {
        perror("crypt");
        exit(1);
    }

    if (strcmp(pass, res->sp_pwdp) == 0)
    {
        printf("check pass!\n");
    }
    else 
        goto l1;

    return 0;
}
