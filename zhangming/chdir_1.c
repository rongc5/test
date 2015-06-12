#include <stdio.h>
#include <unistd.h>

int main(int c, char **v)
{
    if (1 == c)
        chdir(NULL);
    else
        chdir(v[1]);

    system("pwd");
    return 0;
}
