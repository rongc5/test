#ifndef __HOLIDAY_DICT_H_
#define __HOLIDAY_DICT_H_

#include "base_reload.h"
#include "sk_def.h"
#include "common_util.h"

class holiday_dict:public reload_inf
{
    public:
        virtual ~holiday_dict();
        holiday_dict();

        virtual int init(const char * path, const char * file, const char *dump_dir);
        virtual int load();
        virtual int reload();
        virtual bool need_reload();
        virtual int dump();
        virtual int destroy();

    public:

        bool is_trade_date(const char * date);

        void get_trade_date(const std::string & date, std::string & trade_date);

        void get_trade_date(std::string & trade_date);

        void get_yearweek(const std::string &in, std::string &out);

    private:
        void set_path (const char* path);

    private:
        char _fullpath[SIZE_LEN_512];
        char _dumppath[SIZE_LEN_512];
        time_t _last_load;
    public:
        // <date>
        //std::unordered_map<std::string, std::string, str_hasher> _date_dict;
        std::unordered_set<std::string, str_hasher> _date_dict;
};



#endif
