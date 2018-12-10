#include <stdio.h>
#include <string.h>
#include <stdlib.h>

enum TEST{
    PD_OTHER       = 0,
    PD_BENDI       = 1,
    PD_TUPIAN      = 2,
    PD_XIAOSHUO    = 3,
    PD_LINGSHEN    = 4,
    PD_XINWEN      = 7,
    PD_SHIPING     = 8,
    PD_SHOUYE      = 9,
    PD_MP3         = 10,

    PD_MAX     = 99
};


int main(int argc, char *argv[])
{
    enum TEST t;
    t = (enum TEST)100;

    printf("%d\n", t);

    return 0;
}
