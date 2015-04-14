#include <stdio.h>
#include <string.h>

int main(void)
{
    char buf[] = "123aaa345a787aa";
    int i, cnt = strlen(buf);
    char * token = "a";
    char * ptr;
    char * tmp = buf;
    printf("%s(%p)\n", buf, buf);
    while (1)
    {
        ptr = strtok(tmp, token);
        if (NULL == ptr)
            break;
        printf("%s(%p)\n", ptr, ptr);
        tmp = NULL;
    }

    printf("*******************\n");
    for (i = 0; i < cnt; i++)
    {
        if (buf[i] == '\0')
            putchar('&');
        else
            putchar(buf[i]);
    }
    printf("\n");

    return 0;
}
