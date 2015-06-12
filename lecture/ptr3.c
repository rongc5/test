#include <stdio.h>

struct stu {
    int id;
};


int main(int argc, char *argv[])
{
    int *ptr;
    int a = 6;
    ptr = &a;

    printf("%d\n", *ptr);

    *ptr  = 9;
    printf("%d, %d\n", *ptr, a);

    struct stu *p, data = {33};
    p = &data;

    printf("%d, %d\n", p->id, (*p).id);

    return 0;
}
