#include "log_process.h"
#include "common_util.h"


void log_process::log_write(LogType type, const char *format, ...)
{
	
    if (type > _conf.type){
        return;
    }

    std::lock_guard<std::mutex> lck (_log_name[type]._mutex);

    va_list ap;

    check_to_renmae(_log_name[type]._name, _conf.file_max_size);
    FILE * fp = fopen(_log_name[type]._name, "a+");
    if (!fp){
        return;
    }

    va_start(ap, format);
    vfprintf(fp, format, ap);
    va_end(ap);
    fprintf(fp, "\n");
    fclose(fp);
}


const log_conf & log_process::get_log_conf()
{
	return _conf;
}

void log_process::get_file_name(LogType type, char dest[], size_t dest_len)
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

void log_process::check_to_renmae(const char *filename, int max_size)
{
	  if (!filename){
        return;
    }

    char tmp[SIZE_LEN_64];
    char path[SIZE_LEN_128];
    struct stat statBuf;
    stat(filename, &statBuf);

    get_timestr(tmp, sizeof(tmp), "%Y%m%d%H%M%S");
    if (max_size && statBuf.st_size >= max_size){
        snprintf(path, sizeof(path), "%s.%s", filename, tmp);
        rename(filename, path);
    }
}


void log_process::init()
{
	 if (_conf.log_path[0] != '0') {
            char buf[SIZE_LEN_256];
            snprintf(buf, sizeof(buf), "mkdir -p %s", _conf.log_path);
            system(buf);         
     }


	int i = LOGFATAL;
	for (; i<LOGSIZE; i++) {
        std::lock_guard<std::mutex> lck (_log_name[i]._mutex);
        get_file_name((LogType)i, _log_name[i]._name, sizeof(_log_name[i]._name));
	}
}



