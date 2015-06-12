#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    char mac[33];
    mac[0] = '\0';
    char ptr[33] = "123456";

    strcpy(mac, ptr);

    printf("%s\n", mac);
}
