#include <stdio.h>
#include <iostream>

struct src_no_t {
    int cmatch;
    int ad_src_no;

    src_no_t()
    {
        Init();
    }

    int Init()
    {
        cmatch = 0;
        ad_src_no = 0;
        return 0;
    }
};


int main(int argc, char *argv[])
{
   struct src_no_t array[12];
    
    printf("%d\n", sizeof(array)/sizeof(struct src_no_t));

    return 0;
}
