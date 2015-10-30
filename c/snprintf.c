#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char buf[5];

    char *ptr = "hello world";

    snprintf(buf, sizeof(buf), "%s", ptr);
    printf("%s\n", buf);

    return 0;
}
