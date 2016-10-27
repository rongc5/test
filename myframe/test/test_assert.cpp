#include "../src/frm_def.h"

#include <stdio.h>

int main(int argc, char *argv[])
{
    
    int i = 1;

    ASSERT(!i, printf("i = %d\n", i));

    return 0;
}

