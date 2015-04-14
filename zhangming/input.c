#include <stdio.h>
#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

int main(void)
{
    char buf[256];
    int ret;
    struct input_event env;
    int fd = open("/dev/input/event2", O_RDONLY);
    while (1)
    {
        ret = read(fd, &env, sizeof(env));
 //       printf("%d\n", ret);
//        perror(" read error\n");
        if (ret > 0)
        printf("value = %d, type = %d, code = %d\n", env.value, env.type, env.code);

//        memset(&env, 0, sizeof(env));
    }

    return 0;
}
