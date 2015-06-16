#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void)
{
    //char buf[120] = "ab abc abcd abcde abcdefgh"; 
    char buf[512] = "123    456"; 
    char * ptmp = strstr(buf, " ");
    int data[2] = {};

    if (ptmp) {
        printf("%s\n", ptmp);
        sscanf(buf, "%u %u", &data[0], &data[1]);

        printf("res ==> %u  %u\n", data[0], data[1]);
    }

    return 0;
}
