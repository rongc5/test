#ifndef __userid_DICT_H_
#define __userid_DICT_H_

#include "base_reload.h"
#include "sk_def.h"
#include "common_util.h"

class userid_dict:public reload_inf
{
    public:
        virtual ~userid_dict();
        userid_dict();

        virtual int init(const char * path, const char * file, const char *dump_dir);
        virtual int load();
        virtual int reload();
        virtual bool need_reload();
        virtual int dump();
        virtual int destroy();
    public:
        bool is_userid_valid(const std::string & userid, const std::string & date);

    private:
        void set_path (const char* path);

        int load_userid_dict(const char * file);

    private:
        char _fullpath[SIZE_LEN_512];
        char _dumppath[SIZE_LEN_512];
        time_t _last_load;
    public:
        //userid, date
        std::unordered_map<std::string, std::string, str_hasher> _userid_date_map;
};


#endif
