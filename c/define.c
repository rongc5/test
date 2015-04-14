#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALLOC(type, buf, num) \
do {\
    buf = (type *)malloc(sizeof(type)*num);\
    if (buf == NULL){\
        perror("malloc");\
        exit(1);\
    }\
    if (buf){\
        printf("I am leaving!\n");\
        break;\
    }\
    printf("I am still exit!\n");\
    memset(buf, 0, sizeof(type)*num);\
}while (0)


int main(void)
{
    char *ptr;
    ALLOC(char, ptr, 1);
    
    //memcpy(ptr, "hello", 6);
    printf("%d  world\n", ptr);

    return 0;
}
