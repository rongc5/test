#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dyamlic.h"

int main(void)
{
    char *p;
    p = getGlAddr();

    sprintf(p, "hello, zm!");

    print(p);

    return 0;
}


