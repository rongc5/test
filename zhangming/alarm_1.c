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
    int a[100];
    int i, j;

    signal(SIGALRM, alarm_handler);

    char c;
    int tmp;
    while (1)
    {
        tmp = 0;
        i = 0;
        j = 0;
        do
        {
            printf("input time second: ");
            fflush(stdout);
            scanf("%d", &a[j]);
            if (a[j])
                j++;
            if (j > 100)
                break;
        }while ((c = getchar()) != '\n');
        fun1(a, j);

        for (i = 0; i < j; i++)
        {
            printf("%d\n", a[i]);
            alarm(a[i] - tmp);
            pause();
            tmp = a[i];
            a[i] = 0;
        }
    } 

    return 0;
}

