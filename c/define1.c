#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PRINT_ERR(ret) \
{   \
    do{\
    switch(ret)\
    {\
        case 1:\
               printf("hello\n");\
               return -1;\
        case 2:\
               return -2;\
        default :\
                 return -3;\
    }\
    }while(0);\
}

int main(void)
{
    PRINT_ERR(1);
    
    int ret =99;
    printf("ret:%d\n", ret);

    return 0;
}
