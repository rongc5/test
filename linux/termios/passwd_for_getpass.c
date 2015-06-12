#include <stdio.h>
#include <unistd.h>
#include <errno.h>

int main(void)
{
    char *ptr;

    if ((ptr = getpass("Enter passwd:")) == NULL)
    {
        printf("errno: %d, %s!\n", errno, strerror(errno));
    }

    printf("passwd is %s\n", ptr);
    
    return 0;
}
