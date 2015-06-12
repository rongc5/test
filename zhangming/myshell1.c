#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define CMDMAXNO 100
#define DELIM " "

typedef void (*method)(char ** argv);

typedef struct builtin_cmd
{
    char *cmd;
    method func;
}b_cmd;

void cd_func(char ** argv);
void exit_func(char ** argv);

void cd_func(char **argv)
{
    char * pwd = argv[1];
    if (pwd != NULL)
        chdir(pwd);
    else
        chdir(getenv("HOME"));
    return;
}

b_cmd cmds[2] = {
    "cd", cd_func,
    "exit", exit_func,
};

void exit_func(char ** argv)
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
    if (ptrline[ret - 1] == '\n')
        ptrline[ret - 1] = '\0';

    return ptrline;
}

static void construct_command(char * line, char ** argv, int *argc)
{
    char * repter;
    int i = 0;
    while (1)
    {
        repter = strtok(line, DELIM);
        if (repter == NULL)
            break;
        argv[i++] = repter;
        line = NULL;
    }
    argv[i] = NULL;
    *argc = i;
    return;
}

int excute_bulitin_cmd(char ** argv, int argc)
{
    int i, flag = 0;

    for (i = 0; i < sizeof(cmds)/sizeof(b_cmd); i++)
    {
        if (strcmp(argv[0], cmds[i].cmd) == 0)
        {
            flag = 1;
            break;
        }
    }
    if (1 == flag)
    {
        cmds[i].func(argv);
        return 1;
    }
    return 0;
}

static void excute_relocated_cmd(char ** argv, int fd)
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
        ret = execvp(argv[0], argv);
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

static int if_relocated(char ** argv, int argc)
{
    int i, flag = 0, fd, ret, ifrelocated = 0;
    char * path;

    for (i = 0; i < argc; i++)
    {
        if (strncmp(argv[i], ">", 1) == 0)
        {
            flag = 1;
            break;
        }else if (strncmp(argv[i], ">>", 2) == 0)
        {
            flag = 2;
            break;
        }
    }
    if (1 == flag)
    {
        if (strlen(argv[i]) == 1 && i == (argc - 2))
        {
            argv[i] = NULL;
            path = argv[i + 1];
            ifrelocated = 1;
        }else if (strlen(argv[i]) > 1 && i == argc - 1)
        {
            path = argv[i] + 1;
            argv[i] = NULL;
            ifrelocated = 1;
        }else
            return -1;
    }else if (flag == 2)
        return 1;

    if (flag == 1 && ifrelocated == 1)
    {
        fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0755);
        if (-1 == fd)
        {
            perror("open");
            return 1;
        }
        excute_relocated_cmd(argv, fd);
        return 1;
    }
    return 0;
}

void excute_common_cmd(char **argv, int argc)
{
    pid_t pid;
    int ret;
    if (if_relocated(argv, argc) != 0)
        return;

    pid = fork();
    if (-1 == pid)
    {
        perror("fork");
        return;
    }
    if (0 == pid)
    {
        ret = execvp(argv[0], argv);
        if (-1 == ret)
        {
            perror("execvp");
            exit(1);
        }
    }

    wait(NULL);
    return;
}

static debug(char ** argv, int argc)
{
    int i;

    for (i = 0; i < argc; i++)
        printf("argc%d:%s\n", i, argv[i]);
}

void excute_command(char * line)
{
    char * argv[CMDMAXNO];
    int argc, ret;
    construct_command(line, argv, &argc);
    ret = excute_bulitin_cmd(argv, argc);
    if (1 == ret)
        return;
    else
    {
        excute_common_cmd(argv, argc);
    }
}

int main(void)
{
    char * line;

    while (1)
    {
        write(1, "%%:", 3);
        line = read_one_line();
        if (line == NULL)
            continue;
        excute_command(line);
    }
}
