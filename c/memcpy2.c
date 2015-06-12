#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    char mac[33];
//    mac[0] = '\0';
    char ptr[10];
    char tmp[10];

    memcpy(mac, "123456", sizeof("123456"));
    memcpy(tmp, "123456", sizeof("123456"));

    printf("%s  %s\n", mac, tmp);
    return 0;
}
