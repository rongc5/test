#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define v_print(fmt, arges...) \
    fprintf(stderr, fmt, ##arges)

int main(void)
{
    v_print("hello!\n");

    return 0;
}

