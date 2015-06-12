#ifndef __COMM_H
#define __COMM_H

typedef struct stu
{
    int packetlen;
    int math;
    int chinese;
    char name[1];
} stu_t;

#define SERVERPATH "/tmp/serverend"

#endif
