#ifndef __HISTORY_QUOTATION_DICT_H_
#define __HISTORY_QUOTATION_DICT_H_

#include "base_reload.h"
#include "sk_def.h"
#include "common_util.h"

class history_quotation_dict:public reload_inf
{
    public:
        virtual ~history_quotation_dict();
        history_quotation_dict();

        virtual int init(const char * path, const char * file, const char *dump_dir);
        virtual int load();
        virtual int reload();
        virtual bool need_reload();
        virtual int dump();
        virtual int destroy();

    private:
        void set_path (const char* path);
        int load_history_quoation(const char * file);

    private:
        char _fullpath[SIZE_LEN_512];
        char _dumppath[SIZE_LEN_512];
        time_t _last_load;
    public:
        // <date, <id, single> >
        std::map<std::string, std::unordered_map<std::string, quotation_t, str_hasher> > _id_dict;
};



#endif