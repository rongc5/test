#include <stdio.h>

struct stu{
    int id;
};

int main(int argc, char *argv[])
{
    /*A * B;*/

    int *ptr;
    /*char *p;*/

    /*struct stu *ptr;*/

    /*int **p;*/

    int a;
    ptr = &a;

    printf("%d\n", sizeof(ptr));

    return 0;
}
