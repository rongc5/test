#ifndef _STRATEGY_CONF_H_
#define _STRATEGY_CONF_H_

#include "base_reload.h"

class strategy_conf:public reload_inf
{
    public:
        virtual ~strategy_conf();
        strategy_conf();

        int init(const char * path, const char * file, const char *dump_dir);
        virtual int load();
        virtual int reload();
        virtual bool need_reload();
        virtual int dump();
        virtual int destroy();

    private:
        void set_path (const char* path);
        void do_parse();

    private:
        std::map<std::string, std::string> _cfg;
        char _fullpath[SIZE_LEN_512];
        char _dumppath[SIZE_LEN_512];
        time_t _last_load;

    public:
        std::string ua_path;
        std::string ua_file;

        std::string id_path;
        std::string id_file;

        std::string financie_path;
        std::string financie_file;

        std::string ban_path;
        std::string ban_file;

        std::string history_single_path;
        std::string history_single_file;

        std::string history_quotation_path;
        std::string history_quotation_file;

        std::string holiday_dict_path;
        std::string holiday_dict_file;

        std::string recommend_dict_path;
        std::string recommend_dict_file;

        std::string plate_dict_split_path;
        std::string plate_dict_split_file;

        std::string plate_dict_path;
        std::string plate_dict_file;

        std::string addr_dict_split_path;
        std::string addr_dict_split_file;

        std::string addr_dict_path;
        std::string addr_dict_file;

        std::string real_quotation_path;
        std::string real_single_path;

    public:
        std::string real_morning_stime;
        std::string real_morning_etime;

        std::string real_afternoon_stime;
        std::string real_afternoon_etime;

        std::string dump_real_time;

        uint32_t req_quotation_millisecond;
        uint32_t req_single_millisecond;

        uint32_t req_http_timeout;

        uint32_t reload_millisecond;

        uint32_t max_reqhttp_num;

        uint32_t real_single_deque_length;

        uint32_t history_quotation_num;

        uint32_t history_single_num;

        uint32_t destroy_idle_millisecond;

        std::vector<uint32_t> real_single_scale;
};



#endif
