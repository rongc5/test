#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <shadow.h>

int main(int c, char **v)
{
    if (c <= 1)
    {
        fprintf(stderr, "Usage...\n");
        exit(1);
    }
    char passwd[256];
    struct spwd *res;
    int ret;
    int i = 0;
    
    res = getspnam((const char*)getenv("USER"));
    if (NULL == res)
    {
        perror("getpnam");
        exit(1);
    }

   while (1)
   {
       printf("input passwd:");
       fflush(stdout);
       ret = read(0, passwd, 256);
       if (passwd[ret - 1] = '\n')
           passwd[ret - 1] = '\0';
       if (strcmp(res->sp_pwdp, (char *)crypt(passwd, res->sp_pwdp)) == 0)
           break;
       else
       {
           i++;
           if (i == 3)
           {
               fprintf(stderr, "错误三次，即将退出!\n");
               exit(1);
           }
           else
               continue;
       }
   }

   setuid(0);
   setgid(0);

   ret = execvp(v[1], &v[1]);
   if (-1 == ret)
   {
       perror("execvp");
       exit(1);
   }
}
