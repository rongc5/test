#include <stdlib.h>
#include <string.h>
#include <stdio.h>


int main(void)
{
    int ret = 100;

    char buf[ret];

    memcpy(buf, "qianfeng", 13);

    printf("%s\n", buf);
    
    return 0;
}
