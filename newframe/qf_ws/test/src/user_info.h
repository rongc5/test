#ifndef __USER_INFO_H__
#define __USER_INFO_H__

#include "common_def.h"

struct user_info
{
    int op;
    char groupid[SIZE_LEN_512];
    char userid[SIZE_LEN_512];
    char passwd[SIZE_LEN_512];
};


#endif
