#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void task1(void)
{
    printf("task1\n");
}

void task2(void)
{
    printf("task2\n");
}

void task3(void)
{
    printf("task3\n");
}
typedef void (*task_func)(void);

typedef struct task{
    int taskid;
    task_func task;
}task_t;

task_t tasks[] = {
    1, task1,
    2, task2,
    3, task3,
};

void alarm_handler(int s)
{
    printf("hello world!\n");
}

int main(void)
{
    int i, sec;
    int ret;
    signal(SIGALRM, alarm_handler);
    while (1)
    {
        printf("input task id, time second: ");
        fflush(stdout);
        scanf("%d%d", &i, &sec);
        if (i > 0 && i < 4)
        {
            alarm(sec);
            ret = alarm(0);
            if (ret != 0)
            {
                alarm(ret);
            }
           /* else 
            {
                alarm()
            }  */
        }
        else
        {
            printf("task no wrong!\n");
        }
    }
}
