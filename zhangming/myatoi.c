#include <stdio.h>
#include <stdlib.h>

int myatoi(const char *str)
{
    if (str == NULL)
        return 0;
    int result = 0;
    int sign = 1;
    if (('0'<= str[0] && str[0] <= '9') || (str[0] == '-' || str[0] == '+'))
    {
        if (str[0] == '+' || str[0] == '-')
        {
            if (str[0] == '-')
            {
                sign = -1;
            }
            str++;
        }
    }

    while ('0' <= *str && *str <= '9')
    {
        result = result*10 +(*str++ - '0');
    }

    return result * sign;
}

int main(void)
{
    char str[100];
    scanf("%s", str);
    printf("%d\n", atoi(str));

    return 0;
}
