#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void alarm_handler(int s)
{
    printf("\nhello world!\n");
}

void fun1(int a[], int n)
{
    int i, j;
    int tmp;
    for (i = 0; i < n - 1; i++)
        for(j = i + 1; j < n; j++)
        {
            if (a[i] > a[j])
            {
                tmp = a[i];
                a[i] = a[j];
                a[j] = tmp;
            }
        }
}
int main(void)
{
    int a[100] = {0};
    int i, j;

    signal(SIGALRM, alarm_handler);

    char c;

    i = 0;
    j = 0;
    int ret = 0;
    int cm = 0;
    int flag;
    while (1)
    {
            flag = 0;
            printf("input time second: ");
            fflush(stdout);
            scanf(" %d", &a[j]);
           
            if (a[j])
            {
                 j++;
                 flag = 1;
            }
            if (j > 100)
                break;

        if (1 == flag && j > 1)
        { 
            ret = alarm(0);
            cm = a[i] - ret;
            ret = i;
            
            for (i; i < j; i++)
            a[i] = a[i] - cm;
            fun1(a, j);
         }
        
        if (a[i])
        {
            printf("%d\n", a[i]);
            alarm(a[i]);
            
            //i++;
            a[i] = 0;
            
        }
    } 

    return 0;
}

