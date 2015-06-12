#include <stdio.h>
#include <unistd.h>

int main(void)
{
    int ret;
#if 1
    ret = chdir("/tmp");
    if (ret == -1)
    {
        perror("chdir");
        return -1;
    }
#endif
    system("ls -al");

    return 0;
}
