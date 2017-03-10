#include "common_def.h"
#include "log_helper.h"


time_t get_date_str(char dest[], size_t dest_len, const char * format)
{
    if (!dest || !dest_len || !format){
        return 0;
    }   

    time_t now;
    struct tm *tm_now;

    time(&now);
    tm_now = localtime(&now);
    strftime(dest, dest_len, format, tm_now);

    return now;
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


void get_proc_name(char buf[], size_t buf_len) {

    if (!buf || !buf_len) {
        return;
    }

    pid_t pid = getpid();
    char exec_file[SIZE_LEN_1024];
    char proc_name[SIZE_LEN_1024];
    snprintf(exec_file, sizeof(exec_file), "/proc/%d/exe", pid);
    int ret = readlink(exec_file, proc_name, sizeof(proc_name));
    if (-1 == ret)
    {
        LOG_WARNING("readlink for proc_name error, pid:%d", pid);
        buf[0] = 0;
        return;
    }
    proc_name[ret] = 0;
    //获取文件名 
    char *p = strrchr(proc_name, '/');
    if (NULL == p)
    {
        buf[0] = 0;
        return;
    }
    snprintf(buf, buf_len, "%s", p + 1);
    return;
}

