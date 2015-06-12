#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    setuid(0);
    setgid(0);
    printf("euid = %d\n", geteuid());
    execl("/bin/bash", "bash", NULL);

    return 0;
}
