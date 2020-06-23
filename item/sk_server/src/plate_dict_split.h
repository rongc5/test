#ifndef _PLATE_DICT_SPLIT_H_
#define _PLATE_DICT_SPLIT_H_

#include "base_reload.h"
#include "sk_def.h"

class plate_dict_split:public reload_inf
{
    public:
        virtual ~plate_dict_split();
        plate_dict_split();

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
    private:
        std::unordered_set<std::shared_ptr<std::string>, str_hasher, str_equaler> _plate_set;
};



#endif
