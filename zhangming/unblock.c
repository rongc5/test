#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

int main(void)
{
    char buf[256];
    int ret;

    ret = fcntl(0, F_GETFL);
    fcntl(0, F_SETFL, ret | O_NONBLOCK);

    while (1)
    {
        ret = read(0, buf, 256);
        if (-1 == ret)
        {
            if (errno == EAGAIN)
            {
                continue;
            }
        }
        buf[ret] = '\0';
        printf("read:%s", buf);
    }
}
