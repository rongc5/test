#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void)
{
    char buf[120] = "31;"; 

    if (strstr(buf, "31")) {
        printf("hello world\n");
    }



#if 0
    for (i = 0; i < len; i++){
        printf("%c", *(src + i));
    }
#endif

    printf("\n");

    return 0;
}
