#include "log_thread.h"
#include "base_singleton.h"

int log_thread::add(log_msg * msg)
{
    if (!msg) {
        return -1;
    }

    int index = msg->str.length() % _conf.bucketlen;

    thread_lock lock(&_mutex[index]);

    _list[index].push_back(msg);

    return 0;
}

int log_thread::put_msg(log_msg *msg)
{
    log_thread * thread = base_singleton<log_thread>::get_instance();
    if (!thread) {
        REC_OBJ<log_msg> rc(msg);
        return -1; 
    }


    return thread->add(msg);
}

void* log_thread::run()
{
    while (get_run_flag())
    {
        clear();
    }

    return NULL;
}


void log_thread::log_write(log_msg * msg)
{
    if (!msg || msg->type > _conf.type){
        return;
    }


    if (_conf.deal_mode & 1) {

        char file_name[SIZE_LEN_128] = {0};
        get_file_name(msg->type, file_name, sizeof(file_name));

        check_to_renmae(file_name, _conf.file_max_size);
        FILE * fp = fopen(file_name, "a+");
        if (!fp){
            return;
        }

        fprintf(fp, "%s\n", msg->str.c_str());

        fclose(fp);
    }

    if (_conf.deal_mode & 1<<1){
        printf("%s\n", msg->str.c_str());
    }

}


void log_thread::get_file_name(LogType type, char dest[], size_t dest_len)
{
    if (!dest || !dest_len){
        return;
    }   

    switch (type)
    {   
        case LOGFATAL:
            snprintf(dest, dest_len, "%s.%s", _conf.prefix_file_name, "ft");
            break;
        case LOGWARNING:
            snprintf(dest, dest_len, "%s.%s", _conf.prefix_file_name, "wn");
            break;
        case LOGNOTICE:
            snprintf(dest, dest_len, "%s.%s", _conf.prefix_file_name, "nt");
            break;
        case LOGTRACE:
            snprintf(dest, dest_len, "%s.%s", _conf.prefix_file_name, "tc");
            break;
        case LOGDEBUG:
            snprintf(dest, dest_len, "%s.%s", _conf.prefix_file_name, "db");
            break;
        default:
            break;
    }   

    return;
}




void log_thread::check_to_renmae(const char *filename, int max_size)
{
    if (!filename){
        return;
    }

    char tmp[SIZE_LEN_64];
    char path[SIZE_LEN_128];
    struct stat statBuf;
    stat(filename, &statBuf);

    get_date_str(tmp, sizeof(tmp), "%Y%m%d%H%M%S");
    if (max_size && statBuf.st_size >= max_size){
        snprintf(path, sizeof(path), "%s.%s", filename, tmp);
        rename(filename, path);
    }
}

void log_thread::clear()
{
    for (int i = 0; i <  _conf.bucketlen; i++) {
        thread_lock lock(&_mutex[i]);

        list<log_msg *>::iterator it;

        for (it = _list[i].begin(); it != _list[i].end(); it++) {
            log_write(*it);
            delete *it;
            *it = NULL;
        } 

        _list[i].clear();

    }

}

const log_conf & log_thread::get_log_conf()
{
    return _conf;
}
