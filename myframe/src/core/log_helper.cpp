#include "log_helper.h"
#include "base_def.h"


log_mgr::log_mgr()
{
    _type = LOGOFF;
    for (uint32_t i = 0; i < sizeof(_loger)/sizeof(_loger[0]); i++){
        _loger[i] = NULL;
    }
    _max_size = 0;
    _max_record = 0;
}

log_mgr::~log_mgr()
{
    for (uint32_t i = 0; i < sizeof(_loger)/sizeof(_loger[0]); i++){
        if (_loger[i])
            delete _loger[i];
    }
}

void log_mgr::init(LogType type, const char *basename, off_t max_size, size_t max_record)
{
    _type = type;
    _max_size = max_size;
    _max_record = max_record;
    snprintf(base_file_name, sizeof(base_file_name), "%s", basename);
    //printf("basename:%s\n", base_file_name);
}

void log_mgr::get_file_name(LogType type, char dest[], size_t dest_len)
{
    if (!dest || !dest_len){
        return;
    }

    switch (type)
    {
        case LOGFATAL:
            snprintf(dest, dest_len, "%s.%s", base_file_name, "ft");
            break;
        case LOGWARNING:
            snprintf(dest, dest_len, "%s.%s", base_file_name, "wn");
            break;
        case LOGNOTICE:
            snprintf(dest, dest_len, "%s.%s", base_file_name, "nt");
            break;
        case LOGTRACE:
            snprintf(dest, dest_len, "%s.%s", base_file_name, "tc");
            break;
        case LOGDEBUG:
            snprintf(dest, dest_len, "%s.%s", base_file_name, "db");
            break;
    }

    return;
}

void log_mgr::log(LogType type, const char * format, ...)
{
    if (type > _type){
        return;
    }

    if (!_loger[type]){
        _loger[type] = new log_helper();
        char tmp[SIZE_LEN_64];
        get_file_name(type, tmp, sizeof(tmp));
        //printf("%s\n", tmp);
        _loger[type]->init(tmp, _max_size, _max_record);
    }

    va_list ap;
    va_start(ap, format);
    _loger[type]->log_write(format, ap);
    va_end(ap);
}

log_helper::log_helper()
{
    _file_name[0] = '\0';
    _max_size = 0;
    _max_record = 0;
    _current_record = 0;
}

void log_helper::init(const char *filename, off_t max_size, size_t max_record)
{
    if (filename){
        snprintf(_file_name, sizeof(_file_name), "%s", filename);
    }
    _max_size = max_size;
    _max_record = max_record;
}

int log_helper::log_write(const char *format,  va_list &ap)
{
    if (_file_name[0] == '\0'){
        return -1;
    } 

    thread_lock lock(&_mutex);

    check_to_renmae();

    FILE * fp = fopen(_file_name, "a+");
    ASSERT_DO(fp != NULL, printf("file_name: %s\n", _file_name));

    vfprintf(fp, format, ap);
    fprintf(fp, "\n");
    fclose(fp);
    if (_max_record) {
        _current_record++;
    }

    return 0;
}

void log_helper::check_to_renmae()
{
    char tmp[SIZE_LEN_64];
    char path[SIZE_LEN_128];
    struct stat statBuf;
    stat(_file_name, &statBuf);     

    //printf("%lu %lu %lu\n", _max_size, _max_record, _current_record);

    get_date_str(tmp, sizeof(tmp), "%Y%m%d%H%M%S");
    if (_max_size && statBuf.st_size >= _max_size){
        snprintf(path, sizeof(path), "%s.%s", _file_name, tmp);
        rename(_file_name, path);
        _current_record = 0;
    }else if (_max_record && _current_record >= _max_record){
        snprintf(path, sizeof(path), "%s.%s", _file_name, tmp);
        //printf("111 %s %s\n", tmp, path);
        rename(_file_name, path);
        _current_record = 0;
    }

    return;
}




