#ifndef __USER_INFO_H__
#define __USER_INFO_H__

#include "common_def.h"

struct user_info
{
    int op;
    int groupid;
    int userid;
    char passwd[SIZE_LEN_512];
};


#endif
