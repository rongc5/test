#ifndef __PROC_DATA_H__
#define __PROC_DATA_H__

#include "base_reload.h"
#include "sk_conf.h"
#include "finance_dict.h"

#include "address_search_index.h"
#include "change_rate_search_index.h"
#include "cir_value_search_index.h"
#include "down_pointer_search_index.h"
#include "end_avg_end10_search_index.h"
#include "end_avg_end20_search_index.h"
#include "end_avg_end30_search_index.h"
#include "end_avg_end5_search_index.h"
#include "end_avg_end_search_index.h"
#include "end_end10_search_index.h"
#include "end_end20_search_index.h"
#include "end_end30_search_index.h"
#include "end_end5_search_index.h"
#include "end_search_index.h"
#include "hqchange_rate_search_index.h"
#include "hqend_hqstart_search_index.h"
#include "hqrange_percent_search_index.h"
#include "hq_sum_change_rate_search_index.h"
#include "hq_sum_range_percent_search_index.h"
#include "hsingle_diff_search_index.h"
#include "hsingle_sum_diff_search_index.h"
#include "jlrgr_search_index.h"
#include "mgsygr_search_index.h"
#include "mgsy_search_index.h"
#include "mgxjgr_search_index.h"
#include "mgxj_search_index.h"
#include "pb_search_index.h"
#include "pe_search_index.h"
#include "plate_search_index.h"
#include "range_percent_search_index.h"
#include "rsingle_diff2_search_index.h"
#include "rsingle_diff_search_index.h"
#include "up_pointer_search_index.h"
#include "value_search_index.h"
#include "yylrgr_search_index.h"
#include "zysrgr_search_index.h"

class ua_dict;
class ban_dict;
class id_dict;
class addr_dict_split;
class addr_dict;
class plate_dict_split;
class plate_dict;
class history_single_dict;
class history_quotation_dict;
class holiday_dict;
class proc_data:public reload_inf
{
    public:
        virtual ~proc_data(){}
        
        static proc_data * instance();

        int init(sk_conf *conf);
        virtual int load();
        virtual int reload();
        virtual bool need_reload();
        virtual int dump();
        virtual int destroy();
        virtual int destroy_idle();

    public:
        void reg_search_index();

        const base_search_index * get_search_index(std::string & key);

    public:
        reload_mgr<ua_dict> * _ua_dict;
        reload_mgr<id_dict> * _id_dict;

        reload_mgr<recommend_dict> * _recommend_dict;

        reload_mgr<ban_dict> * _ban_dict;

        reload_mgr<addr_dict_split> * _addr_dict_split;

        reload_mgr<addr_dict> * _addr_dict;

        reload_mgr<plate_dict_split> * _plate_dict_split;

        reload_mgr<plate_dict> * _plate_dict;

        reload_mgr<history_single_dict> * _hsingle_dict;

        reload_mgr<history_quotation_dict> * _hquoation_dict;

        reload_mgr<holiday_dict> * _holiday_dict;

        sk_conf * _conf;

        finance_dict  _finance_dict;

        std::unordered_map<std::string, std::shared_ptr<single_vec>, str_hasher> _rsingle_real_dict;

        std::unordered_map<std::string, std::shared_ptr<quotation_t>, str_hasher> _rquoation_real_dict;

    public:
        //index
        ToBufferMgr<std::unordered_map<std::string, std::shared_ptr<finance_t>, str_hasher> > _finance_dict_index;

        ToBufferMgr<std::unordered_map<std::string, std::deque<std::shared_ptr<single_vec> >, str_hasher> > _rsingle_dict_index;

        ToBufferMgr<std::unordered_map<std::string, std::shared_ptr<quotation_t>, str_hasher> > _rquoation_dict_index;

        ToBufferMgr<std::unordered_map<std::string, std::shared_ptr<technical_t>, str_hasher> > _rtechnical_dict_index;

    public:
        //search index;
        ToBufferMgr<std::unordered_set<std::string, str_hasher> > _block_set;

        end_search_index _end_index;
        change_rate_search_index _change_rate_index;
        range_percent_search_index _range_percent_index;

        down_pointer_search_index _down_pointer_index;
        up_pointer_search_index _up_pointer_index;
        end_avg_end_search_index _end_avg_end_index;
        end_avg_end5_search_index _end_avg_end5_index;
        end_avg_end10_search_index _end_avg_end10_index;
        end_avg_end20_search_index _end_avg_end20_index;
        end_avg_end30_search_index _end_avg_end30_index;

        end_end5_search_index _end_end5_index;
        end_end10_search_index _end_end10_index;
        end_end20_search_index _end_end20_index;
        end_end30_search_index _end_end30_index;

        pe_search_index _pe_index;
        pb_search_index _pb_index;
        value_search_index _value_index;
        cir_value_search_index _cir_value_index;

        mgxj_search_index _mgxj_index;
        mgsy_search_index _mgsy_index;
        mgsygr_search_index _mgsygr_index;
        mgxjgr_search_index _mgxjgr_index;
        zysrgr_search_index _zysrgr_index;
        yylrgr_search_index _yylrgr_index;
        jlrgr_search_index _jlrgr_index;

        address_search_index _address_index;
        plate_search_index _plate_index;

        rsingle_diff_search_index _rsingle_diff_index;
        // 
        rsingle_diff2_search_index _rsingle_diff2_index;

        //date, single, id
        hsingle_diff_search_index _hsingle_diff_index;
        //date, sum_single, id
        hsingle_sum_diff_search_index _hsingle_sum_diff_index;

        // history quotation 
        // date, -, id
        hqchange_rate_search_index _hqchange_rate_index;
        hqrange_percent_search_index _hqrange_percent_index;

        hq_sum_range_percent_search_index _hq_sum_range_percent_index;
        hq_sum_change_rate_search_index _hq_sum_change_rate_index;

        hqend_hqstart_search_index _hqend_hqstart_index;

    public:
        char proc_name[SIZE_LEN_256];
        std::string _trade_date;

    private:
        std::map<std::string, base_search_index *> _search_index_map;

    private:
        static proc_data* _singleton;
};


#endif

