#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    char buf[20] = "hello world!";
    char tmp[20];

    strncpy(tmp, "1234567891011121314151617181920", 19);

    printf("%s\n", tmp);
    printf("%p %p\n", tmp, buf);

    return 0;
}
