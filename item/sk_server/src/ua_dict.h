#ifndef UA_DICT_H_
#define UA_DICT_H_

#include "base_reload.h"

class ua_dict:public reload_inf
{
    public:
        virtual ~ua_dict(){}

        virtual int init(const char * path, const char * file, const char *dump_dir);
        virtual int load();
        virtual int reload();
        virtual bool need_reload();
        virtual int dump();
        virtual int destroy();

    private:
        void set_path (const char* path);

    private:
        char _fullpath[SIZE_LEN_512];
        char _dumppath[SIZE_LEN_512];
        time_t _last_load;
    public:
        std::vector<std::string> _ua_vec;
};



#endif
