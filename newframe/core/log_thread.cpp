#include "log_thread.h"

int log_thread::add_msg(base_passing_msg * p_msg)
{
    if (!p_msg) {
        return -1;
    }

    int index = pthread_self() % _channel_vec.size();

    event_channel_msg * msg = _channel_msg_vec[index];
    if (msg && msg->_queue)
        msg->_queue->push(p_msg);

    return 0;
}

virtual bool handle_msg(base_passing_msg * msg)
{
    switch (msg->_op) {
        case PASSING_LOG:
            {
                log_msg * lmsg = dynamic_cast<log_msg *>msg;
                if (lmsg) {
                    log_write(lmsg);
                }
            }
            break;
        default:
            break;
    }

    return false;
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
            snprintf(dest, dest_len, "%s/%s.%s", _conf.log_path, _conf.prefix_file_name, "ft");
            break;
        case LOGWARNING:
            snprintf(dest, dest_len, "%s/%s.%s", _conf.log_path, _conf.prefix_file_name, "wn");
            break;
        case LOGNOTICE:
            snprintf(dest, dest_len, "%s/%s.%s", _conf.log_path, _conf.prefix_file_name, "nt");
            break;
        case LOGTRACE:
            snprintf(dest, dest_len, "%s/%s.%s", _conf.log_path, _conf.prefix_file_name, "tc");
            break;
        case LOGDEBUG:
            snprintf(dest, dest_len, "%s/%s.%s", _conf.log_path, _conf.prefix_file_name, "db");
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

const log_conf & log_thread::get_log_conf()
{
    return _conf;
}

