#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BASE 300000000
typedef long long unsigned int ull_t;

ull_t g_var = 0;

void do_prime(int s)
{
    ull_t i;
    int flag = 0;

    for (i = 2; i < s + BASE; i++)
    {
       if ((s + BASE) % i == 0) 
       {
           flag = 1;
           break;
       }
    }
    if (flag == 1)
    printf("\n%llu is not a prime!\n", (ull_t)(s + BASE));
    else
        printf("\n%llu is a prime!\n", (ull_t)(s + BASE));
}

int main(void)
{
    int k;
    pid_t pid;

    while (1)
    {
        printf("输入数字: ");
        fflush(stdout);
        scanf("%d", &k);
        pid = fork();
        if (-1 == pid)
        {
            perror("fork");
            exit(1);
        }
        if (0 == pid)
        {
            do_prime(k);
            exit(0);
        }
    }
    return 0;
}
