#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    char *ptr;

    while (1)
    {
        ptr = (char *)malloc(sizeof(1024));
        if (ptr == NULL){
            continue;
        }

        free(ptr);
    //    sleep(1);
    }
    return 0;
}
