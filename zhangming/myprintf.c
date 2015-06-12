#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

int myprintf(const char *fmt, ...)
{ 
    int num = 0, i = 0;
    va_list ap;
    char *p = fmt;
    char c;
    char buf[100];

    while (*p != '\0')
    {
        if (*p == '%')
        {
            p++;
            switch(*p)
            {
                case 'c':{
                             buf[i] = 'c';
                             i++;
                         }
                         break;
                case 'd':
                         {
                             buf[i] = 'd';
                             i++;
                         }
                         break;
                case 'p':
                         {
                             buf[i] = 'p';
                             i++;
                         }
                         break;
                case 's':
                         {
                             buf[i] = 's';
                             i ++;
                         }
                         break;
                case 'f':
                         {
                             buf[i] = 'f';
                             i++;
                         }
                         break;
                case '%':
                         {
                             buf[i] = '%';
                             i++;
                         }
                         break;
                default:
                         break;
            }
        }
        p++;
    }
    num = i + 1;
    va_start(ap, num);

    if (0 == i)
    {
        p = fmt;
        while (*p != '\n')
        {
            write(1, p, 1);
            p++;
        }
    }
    else
    {
        p = fmt;
        while (*p != '%' && *p != '\0')
        {
            write(1, p, 1);
            p++;
        }
        for (i = 0; i < num; i++)
        {
            switch (buf[i])
            {
                case 'c':{
                             write(1, (char)va_arg(ap, int), sizeof(char));
                         }
                         break;
                case 'd':
                         {
                             write(1, va_arg(ap, int), sizeof(int));
                         }
                         break;
                case  'f':
                         {
                             write(1, (float)va_arg(ap, double),
                                     sizeof(float));
                         }
                         break;
                case  '%':
                         {
                             write(1, "%", 1);
                         }
                         break;
                case   's':
                         {
                             write(1, (char *)va_arg(ap, int *),
                                     sizeof(int *));
                         }
                         break;
                case   'p':
                         {
                             write(1, va_arg(ap, int*), sizeof(int *));
                         }
                         break;

            }      
            while (*p != '%' && *p != '\0')
            {
                write(1, p, 1);
                p++;
            }
        } 
        while (*p != '%' && *p != '\0')
        {
            write(1, p, 1);
            p++;
        }
    }

    va_end(ap);

    return (p - fmt);
}
int main(void)
{
    myprintf("输出为:%d", 3);

    return 0;
}
