#include "common_def.h"
#include "log_helper.h"


bool operator<(const ObjId & oj1, const ObjId & oj2)
{
    if (oj1._id != oj2._id){
        return oj1._id < oj2._id;
    } else if (oj1._thread_index != oj2._thread_index) {
        return oj1._thread_index < oj2._thread_index;
    }else 
        return false;
}


bool operator==(const ObjId & oj1, const ObjId & oj2)
{
    return oj1._thread_index ==  oj2._thread_index && oj1._id == oj2._id;
}

