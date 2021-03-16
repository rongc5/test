#ifndef _ID_DICT_H_
#define _ID_DICT_H_

#include "base_reload.h"
#include "sk_def.h"

class id_dict:public reload_inf
{
    public:
        virtual ~id_dict();
        id_dict();

        int init(const char * path, const char * file, const char *dump_dir);
        virtual int load();
        virtual int reload();
        virtual bool need_reload();
        virtual int dump();
        virtual int destroy();

        size_t size();

    private:
        void set_path (const char* path);

    private:
        char _fullpath[SIZE_LEN_512];
        char _dumppath[SIZE_LEN_512];
        time_t _last_load;
    public:
        std::vector<std::string> _id_vec;
        std::unordered_map<std::string, std::string, str_hasher> _id_name_map;
};



#endif
