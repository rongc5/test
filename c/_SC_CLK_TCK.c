#include <stdio.h>
#include <unistd.h>

int main(void)
{
    printf("%dHZ\n", sysconf(_SC_CLK_TCK));
    return 0;
}
