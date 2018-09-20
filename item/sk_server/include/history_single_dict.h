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

        virtual int init(const char * path, const char * file, const char *dump_dir);
        virtual int load();
        virtual int reload();
        virtual bool need_reload();
        virtual int dump();
        virtual int destroy();

        static void creat_key(const std::string & date, const std::string & id, std::string & key);

        void get_last_date(int n, std::string & date);

    private:
        void set_path (const char* path);
        int load_history_single(const char * file);

        void update_sum_index();

    private:
        char _fullpath[SIZE_LEN_512];
        char _dumppath[SIZE_LEN_512];
        time_t _last_load;
    public:
        // <date_id, diff> >
        std::unordered_map<std::string, std::shared_ptr<single_vec>, str_hasher> _date_dict;
        // date_id, sum_diff
        std::unordered_map<std::string, std::shared_ptr<single_vec>, str_hasher> _date_sum_dict;
        //id, date,
        std::unordered_map<std::string, std::set<std::string>, str_hasher> _id_date_dict;
        //date
        std::set<std::string> _date_index;
};



#endif
