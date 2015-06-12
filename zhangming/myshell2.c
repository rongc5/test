#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define CMDMAXNO 100
#define DELIM " "

typedef void (*method)(char **v);

typedef struct builtin_cmd
{
    char * cmd;
    method func;
}b_cmd;

void cd_func(char **v);
void exit_func(char **v);

b_cmd cmds[2] = {
    "cd", cd_func,
    "exit", exit_func,
};

void cd_func(char **v)
{
    char * pwd = v[1];
    if (pwd != NULL)
        chdir(pwd);
    else
        chdir(getenv("HOME"));
    return;
}

void exit_func(char **v)
{
    exit(0);
}

char * read_one_line(void)
{
    char * ptrline = NULL;
    size_t n;
    int ret;

    ret = getline(&ptrline, &n, stdin);
    if (-1 == ret)
    {
        if (ferror(stdin))
        {
            perror("getline");
            return NULL;
        }
    }
    if (ptrline[ret-1] == '\n')
        ptrline[ret-1] = '\0';

    return ptrline;
}

static void construct command(char * line, char **v, int *c)
{
    char * reptr;
    int i = 0;
    while (1)
    {
        reptr = strtok(line, DELIM);
        if (reptr == NULL)
            break;
        v[i++] = reptr;
        line = NULL;
    }

        v[i] = NULL;
        *c = i;
        return;
}

int excute_bulitin_cmd(char **v, int c)
{
    int i, flag = 0;

    for (i = 0; i < sizeof(cmds)/sizeof(b_cmd); i++)
    {
        if (strcmp(v[0], cmds[i].cmd == 0))
        {
            flag = 1;
            break;
        }
    }
    if (flag == 1)
    {
        cmds[i].func(v);
        return 1;
    }
    return 0;
}

static void excute_relocated_cmd(char **v, int fd)
{
    pid_t pid;
    int ret;

    pid = fork();
    if (-1 == pid)
    {
        perror("fork");
        return;
    }

    if (0 == pid)
    {
        dup2(fd, 1);
        ret = execvp(v[0], v);
        if (ret == -1)
        {
            perror("execvp");
            exit(1);
        }
    }

    wait(NULL);
    close(fd);
    return;
}

static int if_relocated(char **v, int c)
{
    int i;
    int flag = 0;
    int fd, ret;
    char *path;
    int ifrelocated = 0;

    for (i = 0; i < c; i++)
    {
         if (strncmp(v[i], ">", 1) == 0) 
         {
                         flag = 1;
                                     break; 
         }
         else if (strncmp(v[i], ">>", 2) == 0)
         {
                         flag = 2;
                                     break;
         }
    }
    if (flag == 1)
    {
        if (strlen(v[i]) == 1 && i == (c-2))
        {
                        v[i] = NULL;
                                    path = v[i+1];
                                                ifrelocated = 1;
        }
        else if (strlen(v[i]) > 1 && i == c-1)
        {
                        path = v[i]+1;
                                    v[i] = NULL;
                                                ifrelocated = 1;
        }
        else
            return -1;
    }
    else if (2 == flag)
        return 1;
}
