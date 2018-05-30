#ifndef __REAL_QUOTATION_DICT_H_
#define __REAL_QUOTATION_DICT_H_

#include "base_reload.h"
#include "sk_def.h"

class real_quotation_dict:public reload_inf
{
    public:
        virtual ~real_quotation_dict();
        real_quotation_dict();

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
        std::unordered_map<std::string, ToBufferMgr<std::map<std::string, std::string> >,str_hasher> _id_dict;
};



#endif
