#include "common_def.h"


int get_date_str(char dest[], size_t dest_len, const char * format)
{
    if (!dest || !dest_len || !format){
        return -1;
    }   

    time_t now;
    struct tm *tm_now;

    time(&now);
    tm_now = localtime(&now);
    strftime(dest, dest_len, format, tm_now);

    return 0;
}


uint64_t GetTickCount()
{
    timeval tm; 
    gettimeofday(&tm, NULL);
    uint64_t now = tm.tv_sec * 1000 + tm.tv_usec/1000;
    return now;
}

bool operator<(const ObjId & oj1, const ObjId & oj2)
{
    if (oj1.obj_id() != oj2.obj_id()){
        return oj1.obj_id() < oj2.obj_id();
    } else if (oj1.thread_index() != oj2.thread_index()) {
        return oj1.thread_index() < oj2.thread_index();
    }else 
        return false;
}




