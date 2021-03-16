#ifndef _STRATEGY_CONF_H_
#define _STRATEGY_CONF_H_

#include "base_reload.h"
using namespace std;

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
        map<string, string> _cfg;
        char _fullpath[SIZE_LEN_512];
        char _dumppath[SIZE_LEN_512];
        time_t _last_load;

    public:
        string ua_path;
        string ua_file;

        string id_path;
        string id_file;

        string financie_path;
        string financie_file;

        string ban_path;
        string ban_file;

        string history_single_path;
        string history_single_file;

        string history_main_funds_path;
        string history_main_funds_file;

        string history_quotation_path;
        string history_quotation_file;

        string history_technical_path;
        string history_technical_file;

        string history_wsingle_path;
        string history_wsingle_file;

        string history_wquotation_path;
        string history_wquotation_file;

        string holiday_dict_path;
        string holiday_dict_file;

        string recommend_dict_path;
        string recommend_dict_file;

        string plate_dict_split_path;
        string plate_dict_split_file;

        string plate_dict_path;
        string plate_dict_file;

        string cir_holder_dict_split_path;
        string cir_holder_dict_split_file;

        string cir_holder_dict_path;
        string cir_holder_dict_file;

        string addr_dict_split_path;
        string addr_dict_split_file;

        string addr_dict_path;
        string addr_dict_file;

        string real_quotation_path;
        string real_single_path;
        string real_technical_path;
        string history_wave_path;
        string real_wtechnical_path;
        string real_wquotation_path;
        string real_wsingle_path;
        string history_wwave_path;

        string real_main_funds_path;

        string real_quotation_api;
        string real_single_api;
        string real_main_funds_api;

        string backup_files_path;
        string backuped_path;

        string backup_stime;

        string userid_dict_path;
        string userid_dict_file;

        string dump_quotation_schema;
        string dump_single_schema;
        string dump_technical_schema;
        string dump_wave_schema;
        string dump_wquotation_schema;
        string dump_wtechnical_schema;
        string dump_wsingle_schema;
        string dump_wwave_schema;
        string dump_main_funds_schema;
        
    public:
        string real_morning_stime;
        string real_morning_etime;

        string real_afternoon_stime;
        string real_afternoon_etime;

        string dump_real_time;

        uint32_t req_quotation_millisecond;
        uint32_t req_single_millisecond;
        uint32_t req_main_funds_millisecond;

        uint32_t req_http_timeout;

        uint32_t reload_millisecond;

        uint32_t max_reqhttp_num;

        uint32_t is_req_single_from_file;
        string req_single_from_file_path;

        uint32_t is_req_main_funds_from_file;
        string req_main_funds_from_file_path;

        uint32_t req_single_num_permillisecond;
        uint32_t req_main_funds_num_permillisecond;

        uint32_t req_single_num_perreq;

        uint32_t real_single_deque_length;

        uint32_t is_req_quotation_from_file;
        string req_quotation_from_file_path;

        uint32_t req_quotation_num_permillisecond;

        uint32_t req_quotation_num_perreq;

        uint32_t real_quotation_deque_length;

        uint32_t history_quotation_num;
        uint32_t history_technical_num;

        uint32_t history_single_num;

        uint32_t history_main_funds_num;

        uint32_t history_wquotation_num;

        uint32_t history_wsingle_num;

        uint32_t destroy_idle_millisecond;

        uint32_t per_day_min_req_circle_times;

        uint32_t per_day_perdump_with_circles;

        uint32_t log_single_deque_length;
        uint32_t log_single_deque_length_max;

        uint32_t log_main_funds_deque_length;
        uint32_t log_main_funds_deque_length_max;

        uint32_t lru_ssr_length;

        float is_not_trade_date_ratio;

        vector<uint32_t> real_single_scale;
};



#endif
