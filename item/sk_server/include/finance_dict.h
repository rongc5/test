#ifndef __FINANCE_DICT_H_
#define __FINANCE_DICT_H_

#include "base_reload.h"
#include "sk_def.h"

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

    private:
        void set_path (const char* path);

    private:
        char _fullpath[SIZE_LEN_512];
        char _dumppath[SIZE_LEN_512];
        time_t _last_load;
    public:
        std::unordered_map<std::string, finance_t, str_hasher> _id_dict;

        std::map<int, std::vector<std::string> > _pe_index;
        std::map<int, std::vector<std::string> > _pb_index;
        std::map<int, std::vector<std::string> > _value_index;
        std::map<int, std::vector<std::string> > _cir_value_index;

        std::map<float, std::vector<std::string> > _mgxj_index;
        std::map<float, std::vector<std::string> > _mgsy_index;
        std::map<float, std::vector<std::string> > _mgsygr_index;
        std::map<float, std::vector<std::string> > _mgxjgr_index;
        std::map<float, std::vector<std::string> > _zysrgr_index;
        std::map<float, std::vector<std::string> > _yylrgr_index;
        std::map<float, std::vector<std::string> > _jlrgr_index;
};



#endif
