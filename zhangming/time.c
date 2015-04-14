#include <time.h>
#include <stdio.h>

int main(void)
{
    time_t cur;

    time(&cur);
    printf("%d seconds!\n", (int)cur);

    return 0;
}
