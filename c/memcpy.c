#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    char mac[33];
//    mac[0] = '\0';
    char ptr[10] = "123456";

    memcpy(mac, ptr, 22);

    printf("%s\n", mac);
}
