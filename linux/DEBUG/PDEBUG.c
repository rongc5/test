#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include <sys/types.h>
//#include <unistd.h>

#define PDEBUG(format, arg...) printf("haier:   line:%d, func:%s, file:%s "format, __LINE__, __func__, __FILE__, ##arg)
/*#define PDEBUG(format, arg...) printf("[__FILE__ __func__  __LINE__]"format, ##arg)*/
//#undef PDEBUG
//#define PDEBUG

int main(void)
{
    int b = 999;
    PDEBUG("%d\n", b);

    PDEBUG("\n");
//    printf("["'__FILE__'"__func__""__LINE__""]\n");
    return 0;
}
