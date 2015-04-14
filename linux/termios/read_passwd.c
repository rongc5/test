#include <stdio.h>
#include <termios.h>
#include <unistd.h>

static char * tips = "Input less than 8 chacters!";



int main(void)
{
    char c;
    char buf[30];
    int i = 0;
    struct termios termo, oldtermo;

    if (tcgetattr(1, &termo) < 0)
        return -1;

    oldtermo = termo;
    
    termo.c_lflag &= ~(ECHO | ICANON);

    termo.c_cc[VTIME] = 0;
    termo.c_cc[VMIN] = 1;

    if (tcsetattr(1, TCSAFLUSH, &termo) < 0)
        return -1;
    
    printf("Enyer your passwd:");
    fflush(stdout);

    while (1)
    {
        read(0, &c, 1);
        if (c == '\n'){
            if (i < 8){
                printf("%s", tips);
                fflush(stdout);
                printf("\n");
                printf("Enyer your passwd:");
                fflush(stdout);
                i = 0;
                continue;
            }else{
            buf[i] = '\0';
            break;
            }
        }else if (c == 127){
            i--;
            if (i < 0){
                i = 0;
                continue;
            }
            write(1, "\b \b", 3);
        }else {
            buf[i] = c;
            i++;
            if (i == sizeof(buf)) {
                printf("输入超出范围!");
                fflush(stdout);
                break;
            }
            write(1, "*", 1);
        }
    }

    printf("输入的密码是:%s\n", buf);
    if (tcsetattr(1, TCSAFLUSH, &oldtermo) < 0)
        return -1;

    return 0;
}
