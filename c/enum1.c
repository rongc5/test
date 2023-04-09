#include <stdio.h>

typedef enum{
    a = 1,
    b = 2
}EO;

int main(int argc, char *argv[])
{
    EO e;

    EO buf[] = {a, b, a};

    printf("%d, %d, %d\n", sizeof(e), sizeof(EO), sizeof(buf));

    return 0;
}

