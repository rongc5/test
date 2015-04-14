#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define JAR "a9"

typedef struct user
{
    char name[20];
    char pwd[10];
}USER;

int main(int c, char **v)
{
    FILE *fp;
    char buf[256];
    char *retptr;
    char name[20];
    char tmp[20];

    if (c != 2) {
        fprintf(stderr, "Usage...\n");
        exit(1);
    }
    if (access(v[1], F_OK))
    {
        printf("用户文件不存在,请创建\n");
        fp = fopen(v[1], "w");
        if (!fp)
        {
            perror("fopen error");
            exit(1);
        }
        USER a;
        printf("请输入创建的用户名:\n");
        scanf("%s",  a.name);
        printf("请输入密码:\n");
        scanf("%s", a.pwd);
        //strcpy(a.pwd, (char *)crypt(tmp, JAR));
        fwrite(&a, sizeof(a), 1, fp);
        fclose(fp);
        exit(0);
    }
    fp = fopen(v[1], "r");
    if (NULL == fp)
    {
        perror("fopen");
        exit(1);
    }
    printf("请输入用户名\n");
    scanf("%s", name);
    printf("请输入用户密码\n");
    scanf("%s", tmp);


    while (1)
    {
        retptr = fgets(buf, 256, fp);
        if (retptr == NULL)
        {
            if (feof(fp))
            {
                break;
            }
            else
            {
                perror("fgets");
                exit(1);
            }
        }

        if (buf[strlen(buf) - 1] == '\n')
        {
            buf[strlen(buf) - 1] = '\0';
        }
        if (strstr( buf, name))
        {
            printf("用户名正确\n");

        }
        if (strstr(buf, tmp)) //(char *)crypt(tmp, JAR)tmp
        {
            printf("密码正确,欢迎进入\n");
            return 0;
        }

    }

    printf("密码不正确!\n");

    return 0;
}
