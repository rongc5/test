#ifndef __TIMER__H
#define __TIMER__H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

typedef void (*task_handler)(int id);

typedef enum {
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

int init_timers(void);
int add_a_job(int taskid, task_handler task, int expires);

#endif
