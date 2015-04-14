#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define TIMERNO 100
typedef void (*task_handler)(int id);

typedef enum
{
    UNCOMPLETED,
    RUNNING,
    COMPLETED
} status_t;

typedef struct timer {
    int taskid;
    task_handler task;
    int expires;
    status_t status;
} Timer_t;

void init_timers(void);
int add_a_job(int taskid, task_handler task, int expires);

static Timer_t g_timers[TIMERNO];
static int g_index = 0;

void timers_reboot(void)
{
    int min;

    min = g_timers[g_index - 1].expires;

    alarm(min);
}

void excute_task(void)
{
    int i, min;

    min = g_timers[g_index - 1].expires;

    for (i = g_index - 1; i >= 0; i--)
    {
        if (g_timers[i].expires == min)
            g_timers[i].task(g_timers[i].taskid);
        else if (g_timers[i].expires > min)
            break;
    }
    g_index = i + 1;
}

void alarm_handler(int s)
{
    excute_task();
    timers_reboot();
}

void init_timer(void)
{
    signal(SIGALRM, alarm_handler);
}

void reset_current_timers(void)
{
    int i, un, co;

    un = alarm(0);
    co = g_timers[g_index-1].expires - un;
    for (i = 0; i < g_index; i++) {
        g_timers[i].expires -= co;
    }
}

int append_a_job(int taskid, task_handler task, int expires)
{
    if (g_index >= TIMERNO)
        return -1;
    g_timers[g_index].taskid = taskid;
    g_timers[g_index].task = task;
    g_timers[g_index].expires = expires;
    g_timers[g_index].status = UNCOMPLETED;
    g_index++;
    return 0;
}

int timer_compare(const void *a, const void *b)
{
    Timer_t *m, *n;

    m = (Timer_t *)a;
    n = (Timer_t *)b;

    if (m->expires > n->expires)
        return -1;
    else if (m->expires < n->expires)
        return 1;
    else
        return 0;
}

void sort_jobs(void)
{
    qsort(g_timers, g_index, sizeof(Timer_t), timer_compare);
}

int add_a_job(int taskid, task_handler task, int expires)
{
    int ret;
    reset_current_timers();
    ret = append_a_job(taskid, task, expires);
    if (ret == -1) {
        fprintf(stderr, "Timer queue was full!\n");
        return -1;
    }
    sort_jobs();
    timers_reboot();
    return 0;
}

void test(int s)
{
    printf("task %d was over!\n", s);
}

int main(void)
{
    int opt, sec;

    init_timer();

    while (1) {
        printf("Input job: ");
        fflush(stdout);
        scanf("%d%d", &opt, &sec);
        add_a_job(opt, test, sec);
    }
    return 0;
}
