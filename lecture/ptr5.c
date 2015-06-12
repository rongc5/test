#include <stdio.h>

struct stu {
    int id;
    char buf[32];
};

int main(int argc, char *argv[])
{
    struct stu *ptr; //int *, 指针的类型, int 指针只想的类型, 
    struct stu a;
    ptr = &a; //
    
    printf("%d\n", sizeof(struct stu));

    printf("%p\n", ptr);
    ptr++;

    printf("%p\n", ptr);

    return 0;
}
