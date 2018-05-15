#ifndef __HISTORY_SINGLE_DICT_H_
#define __HISTORY_SINGLE_DICT_H_

#include "base_reload.h"
#include "sk_def.h"
#include "common_util.h"

class history_single_dict:public reload_inf
{
    public:
        virtual ~history_single_dict();
        history_single_dict();

        virtual int init(const char * path, const char * file, uint32_t day_num, const char *dump_dir);
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
        uint32_t _day_num;
        std::string _last_date;;
    public:
        // <id, <date, single> >
        std::unordered_map<std::string, std::map<std::string, std::vector<int> >, str_hasher> _id_dict;
};



#endif
