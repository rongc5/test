#ifndef __LOG__BASE__H_
#define __LOG__BASE__H_


#include "base_def.h"
#include "base_reload.h"

using namespace std;

enum LogType {
    LOGTYPEFATAL = 1,
    LOGTYPEWARNING = 2,
    LOGTYPENOTICE = 4,
    LOGTYPETRACE = 8,
    LOGTYPEDEBUG = 16, 
    LOGTYPESIZE
};

enum LogModel {
    LOGLOCAL = 1,
    LOGTHREAD = 2,
    LOGMODELSIZE
};

class log_conf:public reload_inf {

    public:
        log_conf(const char * sk_conf);
        virtual int load();
        virtual int reload();
        virtual bool need_reload();
        virtual int dump();
        virtual int destroy();

    private:
        void do_parse();

    public:
        uint32_t file_max_size;
        string log_path;
        string prefix_file_name;
        LogType type;
        LogModel model;
        string _dumppath;

        string _log_name[LOGTYPESIZE];
    private:
        void get_file_name(LogType type);


    private:
        map<string, string> _cfg;
        string _log_conf_filename;
        time_t _last_load;
};



#endif
