#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int main(int c, char **v)
{   
    if (c != 2){
        printf("...\n");
        return -1;
    }

    int ret = atoi(v[1]);
    assert(!ret);

    return 0;
}
