#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int main(void)
{
#if defined(A) || defined(B) || defined(C)
    printf("yes!\n");
#else
    printf("no!\n");
#endif

    return 0;
}
