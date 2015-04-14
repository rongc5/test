#include <stdio.h>
#include <unistd.h>

int main(void)
{
    char buf[512];

    getcwd(buf, 512);
    printf("%s\n", buf);

    return 0;
}
