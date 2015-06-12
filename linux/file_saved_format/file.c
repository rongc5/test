#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iconv.h>
#include <ctype.h>

void prase(char *src)
{
    char *p;
    int len = 0, i;

    len = strlen(src);
    p = src + len -1;

    for (i = 0; i < 2; i++){
        if (*p == '\r')
            *p = ',';
        p--;
    }
    
}



int main(int c, char **v)
{
    if (c != 3){
        perror("usage...");
        return -1;
    }

    char buf[256];
    char tmpbuf[256];
    char *rptr, *tmp, *p;
    int i = 0, ret, n;


    FILE *rp = fopen(v[1], "r");
    if (rp == NULL){
        perror("fopen...");
        return -1;
    }

    //sprintf(buf, "%s.new", v[1]);
    FILE *wp = fopen(v[2], "w+");
    if (wp == NULL){
        perror("fopen...");
        return -1;
    }

    while (1)
    {
        memset(buf, 0, sizeof(buf));
        rptr = NULL;
        rptr = fgets(buf, 256, rp);
        if (rptr == 0){
            if (feof(rp))
                break;
        }

        prase(buf);
        if (buf[strlen(buf) - 1] == '\n')
            buf[strlen(buf) - 1] = '\0';

        printf("%s\n", buf);
#if 0
        ret = my_iconv((char*)"utf8", (char*)"gbk", tmpbuf, sizeof(tmpbuf), buf, 256);
        if (ret == 1){
            return -1;
        }
        //printf("%s\n", tmpbuf);
#endif
        fprintf(wp, "%s\n", buf);
    }


    fclose(rp);
    fclose(wp);

    return 0;
}
