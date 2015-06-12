#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct stu{
    int a;
    char b[];
};

struct data{
    char a;
    double b;
};

int main(void)
{
    struct stu a;
    struct data b;
    printf("%d, %d\n", sizeof(struct stu), sizeof(b));

    return 0;
}
