#include <stdio.h>

int main(void)
{
    char buf[32];
    int ret;

    while (1)
    {
        ret = fread(buf, sizeof(char), 3, stdin);
        fwrite(buf, sizeof(char), ret, stdout);
        fflush(stdout);
    }
    return 0;
}
