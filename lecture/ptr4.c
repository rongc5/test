#include <stdio.h>

struct stu {
    int id;
};

int main(int argc, char *argv[])
{
    int *ptr; //int *, 指针的类型, int 指针只想的类型, 
    int a = 6;
    ptr = &a; //

    printf("%p, %p\n", &ptr, ptr);

    return 0;
}
