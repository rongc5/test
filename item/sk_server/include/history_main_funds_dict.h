#ifndef __HISTORY_main_funds_DICT_H_
#define __HISTORY_main_funds_DICT_H_

#include "base_reload.h"
#include "sk_def.h"
#include "common_util.h"

class history_main_funds_dict:public reload_inf
{
    public:
        virtual ~history_main_funds_dict();
        history_main_funds_dict();

        virtual int init(const char * path, const char * file, const char *dump_dir);
        virtual int load();
        virtual int reload();
        virtual bool need_reload();
        virtual int dump();
        virtual int destroy();
        void update();

        void update_real_main_funds(const std::string & trade_date, const std::string & id, std::shared_ptr<main_funds_t> main_funds);

        void update_search_index();
    private:
        void update_hmain_funds_search();
        void update_rmain_funds_search();

        void dump_real_main_funds();

    private:
        void set_path (const char* path);
        int load_history_main_funds(const char * file);

    private:
        char _fullpath[SIZE_LEN_512];
        char _dumppath[SIZE_LEN_512];
        time_t _last_load;
    public:
        // <date_id, diff> >
        std::unordered_map<std::string, std::deque<std::shared_ptr<main_funds_t> >, str_hasher> _date_dict;
        //id, date,
        std::unordered_map<std::string, std::set<std::string>, str_hasher> _id_date_dict;
    private:
        std::string _last_date;
};



#endif
