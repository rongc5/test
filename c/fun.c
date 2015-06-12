#include <stdio.h>

void fun(const char * s1, const char *s2)
{
    for (; *s1 != '\0' && *s2 != '\0'; )  //将两个字符串的单词按先后顺序打印出来
    {

        if ((*s1 >= 'a' && *s1 <= 'z') || (*s1 >= 'A' && *s1 <= 'Z'))   //第一个字符串单词的打印
        {
            for (; *s1 != '\0' && *s1 != ' '; s1++) //第一个字符串单词结束判断
                printf("%c", *s1);
        }
        else
            s1++;

        printf(" ");  


        if ((*s2 >= 'a' && *s2 <= 'z') || (*s2 >= 'A' && *s2 <= 'Z'))  //第二个字符串单词的打印
        {
            for (; *s2 != '\0' && *s2 != ' '; s2++)//第二个字符串单词结束判断
                printf("%c", *s2);
        }
        else
            s2++;

        printf(" ");
    }

    printf(" ");
    if (*s1 != '\0')  //第一个字符串比第二个字符串长情况
        printf("%s", s1);

    printf(" ");
    if (*s2 != '\0')  //第二个字符串比第一个字符串长情况
        printf("%s", s2);

    printf("\n");
}

int main(int argc, char *argv[])
{
    char * a = "welcome to qianfeng";

    char * b = "hello world";

    fun(a, b);

    return 0;
}
