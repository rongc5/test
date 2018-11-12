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

        static void creat_key(const std::string & date, const std::string & id, std::string & key);

        //查看过去几天的日期
        void get_last_date(int n, std::string & date);

    private:
        void set_path (const char* path);
        int load_history_quoation(const char * file);

        void update_sum_index();

    private:
        char _fullpath[SIZE_LEN_512];
        char _dumppath[SIZE_LEN_512];
        time_t _last_load;
    public:
        // <date_id, quotation> >
        std::unordered_map<std::string, std::shared_ptr<quotation_t>, str_hasher> _id_dict;

        // <date_id, sum_quotation> >
        std::unordered_map<std::string, std::shared_ptr<sum_quotation>, str_hasher> _id_sum_dict;
        //id, date,
        std::unordered_map<std::string, std::set<std::string>, str_hasher> _id_date_dict;
        //date
        std::set<std::string> _date_index;
};



#endif
