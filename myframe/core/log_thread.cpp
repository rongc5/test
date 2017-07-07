#include "log_thread.h"

void log_thread::put_msg(ObjId & id, normal_msg * p_msg)
{
    if (!p_msg) {
        return ;
    }

    log_msg * lmsg = dynamic_cast<log_msg *>(p_msg);
    if (!lmsg){
       REC_OBJ<log_msg> rc(lmsg);
       return;
    }

    //PDEBUG("%lu _channel_msg_vec.size:%d\n", lmsg->tid, _channel_msg_vec.size());
    int index = lmsg->tid % _channel_msg_vec.size();

    event_channel_msg * msg = _channel_msg_vec[index];
    msg->_base_obj->process_recv_msg(id, msg);
    write(msg->_channelid, CHANNEL_MSG_TAG, sizeof(CHANNEL_MSG_TAG));

    return ;
}

void log_thread::handle_msg(normal_msg * msg)
{
    REC_OBJ<normal_msg> rec(msg);
    log_msg * lmsg = dynamic_cast<log_msg *>(msg);
    if (lmsg) {
        log_write(lmsg);
    }
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

