#include <termios.h>
#include <unistd.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int read_passwd(char *passwd)
{
    struct termios term, oldterm;
    char c, buf[256];
    int ret, i = 0;

    ret = tcgetattr(1, &term);
    if (ret < 0)
    {
        perror("tcgetattr");
        return -1;
    }

    oldterm = term;
    term.c_lflag &= ~(ECHO | ICANON);
    term.c_cc[VTIME] = 0;
    term.c_cc[VMIN] = 1;

    ret = tcsetattr(1, TCSAFLUSH, &term);
    if (ret < 0)
    {
        perror("tcsetattr");
        return -1;
    }

    printf("输入密码：");
    fflush(stdout);

    while (1)
    {
        ret = read(0, &c, 1);
        if (c == '\n')
        {
            if (i < 8)
            {
                printf("\n输入的字符数小于8个,重新输入: ");
                fflush(stdout);
                i = 0;
                continue;
            }else
                break;
        }
        else if (c == 127)
        {
            i--;
            if (i < 0)
                i = 0;
            else 
            {
                write(1, "\b \b", 3);
                continue;
            }
        }
        else 
        {
            write(1, "*", 1);
            buf[i++] = c;
            if (i == 256)
            {
                printf("密码太长!\n");
                break;
            }
        }
    }

    buf[i] = 0;

    strcpy(passwd, (char *)crypt(buf, "12"));
    ret = tcsetattr(1, TCSAFLUSH, &oldterm);
    if (ret < 0)
    {
        perror("tcsetattr");
        return -1;
    }
    return 0;
}

int main(void)
{
    char passwd[256];
    read_passwd(passwd);

    printf("\n%s\n", passwd);

    return 0;
}
