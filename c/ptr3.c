#include <stdio.h>
#include <stdlib.h>

struct stu{
    char id;
    int math;
    char name[32];
};

int main(int argc, char *argv[])
{
    struct stu *ptr, a, b[2] = {{1, 99, "zhangsan"}, {2, 98, "lisi"}};

    ptr = b;

    printf("%p, %d %d %s\n", ptr, ptr->id, ptr->math, ptr->name); 
    ptr++;
    printf("%p, %d %d %s\n", ptr, ptr->id, ptr->math, ptr->name); 

    printf("%d\n", sizeof(struct stu));

    return 0;
}
