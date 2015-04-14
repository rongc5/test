#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *parseName(char *buf)
{
    int i = 0;
    while (buf[i++] != ':');
    buf[i-1] = '\0';
    char *ptr = (char *)malloc(strlen(buf) + 1);
    if (!ptr)
    {
        perror("malloc");
        return NULL;
    }

    strcpy(ptr, buf);
    return ptr;
}

char * get_userName(uid_t uid)
{
    FILE *fp;
    char buf[512];
    char * retptr = NULL;
    char token[10];

    sprintf(token, "%d", (int)uid);

    fp = fopen("passwd", "r");
    if (!fp)
    {
        perror("fopen");
        exit(1);
    }

    while (1)
    {
        retptr = fgets(buf, 512, fp);
        if (retptr == NULL)
        {
            if (feof(fp))
                break;
            else
            {
                perror("fgets");
                exit(1);
            }
        }
        if (buf[strlen(buf)-1] == '\n')
            buf[strlen(buf)-1] = '\0';
        if (strstr(buf, token))
        {
            return parseName(buf);
        }
    }

    fclose(fp);

    return NULL;
}

int main(void)
{
    char printbuf[512];
    char *cur = printbuf;
    int ret;

    char *name = NULL;
    name = get_userName(1000);

    ret = sprintf(cur, "%c", '-');
    cur += ret;
    ret = sprintf(cur, "rwx-rw-rw-");
    cur += ret;
    sprintf(cur, "%s", name);
    printf("%s\n", printbuf);
    free(name);

    return 0;
}

