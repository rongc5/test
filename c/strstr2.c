#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void)
{
    char buf[120] = "31    5678    999"; 
    char * ptr = NULL;

    ptr = strstr(buf, " ");

    if (ptr) {
        printf("%s\n", ptr);
    }



#if 0
    for (i = 0; i < len; i++){
        printf("%c", *(src + i));
    }
#endif

    return 0;
}
