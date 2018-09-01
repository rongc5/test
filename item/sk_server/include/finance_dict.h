#ifndef __FINANCE_DICT_H_
#define __FINANCE_DICT_H_

#include "base_reload.h"
#include "sk_def.h"
#include "common_util.h"

class finance_dict:public reload_inf
{
    public:
        virtual ~finance_dict();
        finance_dict();

        virtual int init(const char * path, const char * file, const char *dump_dir);
        virtual int load();
        virtual int reload();
        virtual bool need_reload();
        virtual int dump();
        virtual int destroy();
        virtual int destroy_idle();

    private:
        void set_path (const char* path);

    private:
        char _fullpath[SIZE_LEN_512];
        char _dumppath[SIZE_LEN_512];
        time_t _last_load;
};



#endif
