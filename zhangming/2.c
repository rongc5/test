#include <hello.h>

int fun(char *src)
{
    int i = 0;

    while (*src != '\n')
    {
        if (*src>'A' && *src<'Z')
            {
                *src = *src + 32;
                i++;
            }
        src++;
    }
    return i;
}

int main(int argc, char* argv[])
{
    int i = 0;
    char str[100];
    int b;
     
    printf("请输入一组字符串！\n");

    do
    {
        scanf("%c", &str[i]);
        i++;
    } while (str[i - 1] != '\n');

    printf("小写转换为大写后的字符串为：\n");

    b =  fun(str);
    str[i - 1] = '\0';

    printf("大写字母个数为%d, %s\n", b, str);

    return 0;
}
          hello