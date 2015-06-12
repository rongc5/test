#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>


int main(void)
{
    char buf[32];
    int ret;
    pid_t pid;
    char *p = "ls";
    char tmp[32];
    char buf_1[32];
    int flac;
    int flag;
    int fd;
    char name[10];
    int ft[2];

    while (1) {
        printf("%%(qianfeng)>");
        fflush(stdout);
        ret = read(0, buf, 32);
        flag = 0;
        flac = 0;
        if (buf[ret-1] == '\n')
            buf[ret-1] = '\0';
             int i = 0, j = 0,k=0;
        if (strstr(buf, " ") != NULL)
        {   
            flag = 1;
           
           while (buf[i] != ' ' && i < ret-1)
                   { 
                      buf_1[i] = buf[i];
                      i++;
                    }
             buf_1[i] = '\0';
             while (buf[i] == ' '&& i < ret-1)
                  i++;
             while (buf[i] != ' ' &&i < ret-1)     
                     tmp[j++] = buf[i++];
                     
                     tmp[j] = '\0';
                     // printf("%s, %s\n", buf_1, tmp);
        }
        if (strstr(buf, ">"))
        {
            flac = 1;
            i = 0;
            while (buf[i] != '>')
             i++;
             i++;
             while (buf[i] == ' ')
             i++;
             while (buf[i] != '\0')
             name[k++] = buf[i++];
             name[k] = '\0';
             fd = creat(name, 0666);
             if (fd == -1)
            {
                perror("重定向open");
                exit(1);
            }
            close (fd);
        }
        if (strstr(buf, "pwd"))
        {
            if (strcmp(buf, "pwd") == 0)
                {
                    flag = 2;
                }
                else
                flag = 3;
                
        }
   
        pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(1);
        }
       
        if (pid == 0) {
            if (0 == flag)
            ret = execlp(buf, buf, NULL);
            else if (1 == flag && flac == 0)
                {
                //fun1(buf);
                ret = execlp(buf_1, buf_1, tmp, NULL);
                }
            else if (1 == flag && flac == 1)
                {
                    fd = open(name, O_RDONLY | O_WRONLY);
                    dup2(fd, 1);
                    //write(1, "dup2", 4);
                    ret = execlp(buf_1, buf_1, tmp, NULL);
                    close(fd);
                }
            else if (2 == flag && flac == 0)
                {
                    execlp("/bin/pwd", buf, NULL);
                } 
            else if (3 == flag && 1 == flac)
                {
                 fd = open(name, O_RDONLY | O_WRONLY);
                 dup2(fd, 1);
                 ret = execlp("/bin/pwd", buf_1, NULL);
                 close(fd);
                }
            if (ret == -1) {
                perror("execlp");
                exit(1);
            }
            exit(0);
        }
        wait(NULL);
    }
    return 0;
}
