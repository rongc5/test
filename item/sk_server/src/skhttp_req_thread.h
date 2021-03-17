#ifndef __SKHTTP_REQ_THREAD_H_
#define __SKHTTP_REQ_THREAD_H_

#include "base_def.h"
#include "base_net_thread.h"
#include "common_util.h"
#include "common_def.h"
#include "log_helper.h"
#include "rquotation_data_process.h"
#include "proc_data.h"
#include "sk_def.h"
#include "ua_dict.h"
#include "id_dict.h"
#include "common_obj_container.h"
#include "rsingle_data_process.h"
#include "holiday_dict.h"

class skhttp_req_thread:public base_net_thread
{
    public:

        skhttp_req_thread();

        virtual void handle_msg(std::shared_ptr<normal_msg> & p_msg);

        void real_req_start();
    
        void do_single();
        void add_single_timer();

        void do_main_funds();
        void add_main_funds_timer();

        void add_destroy_idle_timer();

        void do_quotation();
        void add_quotation_timer();

        void first_in_day();

        bool is_real_time(uint32_t req_circle_times);

        void req_real_quotation(std::shared_ptr<std::set<std::string>> id_vec);

        void req_real_single(std::string id);

        void req_real_main_funds(std::string id);

        virtual void handle_timeout(std::shared_ptr<timer_msg> & t_msg);

        virtual void run_process();

    public:

        void reload_timer_start();

        void destroy_idle_start();

    protected:
        time_t get_real_time(const char * date, const char * time);

        bool need_dump_real_quotation();
        bool need_dump_real_single();
        bool need_dump_real_main_funds();

        bool need_update_quotation_dict();
        bool need_update_single_dict();
        bool need_update_main_funds_dict();

        bool need_update_wquotation_dict();
        bool need_update_wsingle_dict();

        void update_week_tdate_search_index();

        void update_block_set_search_index();

        void update_lrussr_search_index();

        bool need_update_holiday_dict();

        //void notify_update_date();

        //void reset_threadid_map_flag();

    protected:
        std::string _req_date;
        std::string _trade_date;
        uint32_t _quotation_index; //request id_index in id_dict
        bool _req_quotation;
        time_t _quotation_circle_start_time;

        uint32_t _single_index;
        bool _req_single;
        time_t _single_circle_start_time;

        uint32_t _main_funds_index;
        bool _req_main_funds;
        time_t _main_funds_circle_start_time;

        uint32_t _quotation_destroy_num;//response id_index in id_dict
        uint32_t _single_destroy_num;
        uint32_t _main_funds_destroy_num;

        time_t real_morning_stime;
        time_t real_morning_etime;
        
        time_t real_afternoon_stime;
        time_t real_afternoon_etime;

        time_t dump_real_time;

        uint32_t _history_quotation_num;
        uint32_t _history_technical_num;
        uint32_t _history_single_num;
        uint32_t _history_main_funds_num;

        uint32_t _history_wquotation_num;
        uint32_t _history_wsingle_num;

        time_t backup_stime;

        bool is_backuped;

        uint32_t _req_quotation_circle_times;//each day req circle times;
        uint32_t _req_single_circle_times;//each day req circle times;
        uint32_t _req_main_funds_circle_times;//each day req circle times;

        id_dict * _id_dic;
        ua_dict * _ua_dic;

        //std::unordered_map<uint32_t, int> _reg_date_threadid_map;
    private:
        bool _is_first;
};





#endif
