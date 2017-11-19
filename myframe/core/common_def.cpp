#include "common_def.h"
#include "log_helper.h"
#include "common_util.h"

log_conf::log_conf()
{
    file_max_size = DEFAULT_LOG_MAX_SIZE;
    strcpy(log_path, "logs");
    get_proc_name(prefix_file_name, sizeof(prefix_file_name));
    type = LOGDEBUG;
}

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

