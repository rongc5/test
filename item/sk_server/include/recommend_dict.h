#ifndef _RECOMMEND_DICT_H_
#define _RECOMMEND_DICT_H_

#include "base_reload.h"
#include "common_util.h"

class recommend_dict:public reload_inf
{
    public:
        virtual ~recommend_dict();
        recommend_dict();

        int init(const char * path, const char * file, const char *dump_dir);
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
        //id, recommend_flag
        std::unordered_multimap<std::string, std::string, str_hasher> _id_dict;
};



#endif
