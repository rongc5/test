#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iconv.h>
#include <ctype.h>

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
#if 0
int isdigit(char c)
{
    if (c >= '0' && c <= '9')
        return 1;
    else
        return 0;
}
#endif
char *mystrtok(char *src, char *token)
{
    char * ps = NULL; 
    static char *save = NULL;
    char * pd = token;
    char * tmpsrc = NULL;
    char * tmptoken = pd;

    if (src != NULL)
        save = src;

    if (save == NULL)
        return NULL;

    for (ps = save, tmpsrc = save; *tmpsrc != '\0'; tmpsrc++, tmptoken = pd){
        for (tmptoken; *tmptoken != '\0' && *tmptoken != *tmpsrc; tmptoken++)
            ;
        if (*tmpsrc == *tmptoken){
            *tmpsrc = '\0';
            save = ++tmpsrc;
            return ps;
        }

    }

    if (*tmpsrc == '\0')
        save = NULL;

    return ps;
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
    FILE *wp = fopen("tmp", "w+");
    if (wp == NULL){
        perror("fopen...");
        return -1;
    }

    while (1)
    {

        i = 0;
        memset(buf, 0, sizeof(buf));
        rptr = NULL;
        rptr = fgets(buf, 256, rp);
        if (rptr == 0){
            if (feof(rp))
                break;
        }

        if (buf[strlen(buf) - 1] == '\n')
            buf[strlen(buf) - 1] = '\0';

        // char str[] = "新园乡    932 枋寮乡  940";
#if 1
        ret = my_iconv((char*)"utf8", (char*)"gbk", tmpbuf, sizeof(tmpbuf), buf, 256);
        if (ret == 1){
            return -1;
        }
        //printf("%s\n", tmpbuf);
#endif
        char *ch = strtok(tmpbuf, v[2]);
        while(ch != NULL)
        {
            i++;

            if (i%2){
                tmp = ch;
            }
            else{
                //printf("%s  %s\n", tmp, ch);
                //ret = atoi(ch);
               // if (ret)
                    fprintf(wp, "%s,%s\n", tmp, ch);
               // else
                    //fprintf(wp, "%s,%s,%s\n", tmp, ch,"000000");
                //else
                    //break;
            }
           // printf("%s\n", ch);
            ch = strtok(NULL, v[2]);
        }
    }

    fclose(rp);
    fclose(wp);


    return 0;
}
