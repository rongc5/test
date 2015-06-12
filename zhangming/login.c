#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAXSIZE 256
#define JAR "99"

void register_process(void)
{
    char name[MAXSIZE];
    char passwd[MAXSIZE];
    FILE * fp;

    printf("输入用户名:\n");
    scanf("%s", name);
    printf("输入密码:\n");
    scanf("%s", passwd);
    strcpy(passwd, (char *)crypt(passwd, JAR));

    fp = fopen("/tmp/passwd", "a+");
    {;}

    fprintf(fp, "%s:%s\n", name, passwd);
    fclose(fp);

    return;
}

int login_process(void)
{
    char name[MAXSIZE];
    char passwd[MAXSIZE];
    FILE * fp;
    char lineptr[MAXSIZE];
    char * retptr;
    char cmp[MAXSIZE];

    printf("输入用户名:\n");
    scanf("%s", name);
    printf("输入密码:\n");
    scanf("%s", passwd);
    strcpy(passwd, (char *)crypt(passwd, JAR));
    sprintf(cmp, "%s:%s", name, passwd);

    fp = fopen("/tmp/passwd", "r");
    {;}

    while (1)
    {
        retptr = fgets(lineptr, MAXSIZE, fp);
        if (retptr == NULL)
            break;
        if (lineptr[strlen(lineptr) - 1] == '\n')
            lineptr[strlen(lineptr) - 1] = '\0';
        if (strcmp(lineptr,  cmp) == 0)
            break;
    }
    fclose(fp);

    if (retptr)
        return 1;
    else 
        return 0;
}

int main(void)
{
    int choice = 0;
    char buf[] = "welcome! input 0 equal register, input 1 equal login\n";
    int ret;
    printf("%s", buf);

    while (1)
    {
        scanf("%d", &choice);
        if (0 == choice)
        {
            register_process();
            break;
        }
        else if (1 == choice)
        {
            ret = login_process();
            if (ret)
                break;
            else
            {
                printf("用户名或者密码错误!\n");
                continue;
            }
            }
        
            else{
                printf("输入错误，重新输入!\n");
                continue;
            }

    }

    printf("hello wrold!\n");

    return 0;
}
