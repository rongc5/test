#include <sys/timeb.h>
#include <stdio.h>

int main(int c, char **v)
{
    struct   timeb   tp;
    ftime(&tp);
    printf("%d\n", tp.time);
    return 0;
}
