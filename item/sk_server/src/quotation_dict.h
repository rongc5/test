#ifndef __QUOTATION_DICT_H_
#define __QUOTATION_DICT_H_

#include "base_reload.h"
#include "inc_dict.h"
#include "sk_def.h"
#include "common_util.h"

class quotation_dict:public reload_inf
{
    public:
        virtual ~quotation_dict(){}

        virtual int init(const char * path, const char * file, uint32_t query_num, const char *dump_dir);
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
        inc_dict_t<ToBufferMgr<quotation_t> > _id_dict;
};



#endif
