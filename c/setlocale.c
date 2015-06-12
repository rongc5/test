#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

int main(void)
{
    printf("%s\n", setlocale(LC_ALL, ""));

    return 0;
}
