#ifndef __COMMON_DEF_H__
#define __COMMON_DEF_H__

#include "base_def.h"

enum normal_msg_op
{
    MSG_CONNECT = 1,
    PASS_NEW_MSG
};

struct ObjId
{
    uint32_t _id;
    uint32_t _thread_index;

    ObjId():_id(0), _thread_index(0){}
};

bool operator < (const ObjId & oj1, const ObjId & oj2);

bool operator==(const ObjId & oj1, const ObjId & oj2);

class normal_msg 
{
    public:
        normal_msg(){}
        virtual ~normal_msg(){}
        normal_msg_op _msg_op;
};


class normal_obj_msg //内部传递的消息
{
    public:
        ObjId _id;
        normal_msg * p_msg;

        virtual ~normal_obj_msg(){
        }
};






#endif
