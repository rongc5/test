#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char buf[12];

    float a = 1.33335;


    snprintf(buf, sizeof(buf), "%.*f", a, 3);
    printf("%s\n", buf);

    return 0;
}
