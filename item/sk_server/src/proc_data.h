#ifndef __PROC_DATA_H__
#define __PROC_DATA_H__

#include "base_reload.h"
#include "sk_conf.h"
#include "real_single_dict.h"
#include "real_quotation_dict.h"

class ua_dict;
class recommend_dict;
class finance_dict;
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

    public:
        reload_mgr<ua_dict> * _ua_dict;
        reload_mgr<id_dict> * _id_dict;

        reload_mgr<recommend_dict> * _recommend_dict;

        reload_mgr<finance_dict> * _finance_dict;

        reload_mgr<ban_dict> * _ban_dict;

        reload_mgr<addr_dict_split> * _addr_dict_split;

        reload_mgr<addr_dict> * _addr_dict;

        reload_mgr<plate_dict_split> * _plate_dict_split;

        reload_mgr<plate_dict> * _plate_dict;
        
        reload_mgr<real_single_dict> * _rsingle_dict;

        reload_mgr<real_quotation_dict> * _rquoation_dict;

        reload_mgr<history_single_dict> * _hsingle_dict;

        reload_mgr<history_quotation_dict> * _hquoation_dict;

        reload_mgr<holiday_dict> * _holiday_dict;

        sk_conf * _conf;


    public:
        //search index;
        ToBufferMgr<std::unordered_set<std::string, str_hasher> > _block_set;

        ToBufferMgr<std::multimap<float, std::string> > _end_index;
        ToBufferMgr<std::multimap<float, std::string> > _change_rate_index;
        ToBufferMgr<std::multimap<float, std::string> > _range_percent_index;

        ToBufferMgr<std::multimap<float, std::string> > _down_pointer_index;
        ToBufferMgr<std::multimap<float, std::string> > _up_pointer_index;
        ToBufferMgr<std::multimap<float, std::string> > _end_avg_price_index;

        ToBufferMgr<std::multimap<int, std::string> > _pe_index;
        ToBufferMgr<std::multimap<int, std::string> > _pb_index;
        ToBufferMgr<std::multimap<int, std::string> > _value_index;
        ToBufferMgr<std::multimap<int, std::string> > _cir_value_index;

        ToBufferMgr<std::multimap<float, std::string> > _mgxj_index;
        ToBufferMgr<std::multimap<float, std::string> > _mgsy_index;
        ToBufferMgr<std::multimap<float, std::string> > _mgsygr_index;
        ToBufferMgr<std::multimap<float, std::string> > _mgxjgr_index;
        ToBufferMgr<std::multimap<float, std::string> > _zysrgr_index;
        ToBufferMgr<std::multimap<float, std::string> > _yylrgr_index;
        ToBufferMgr<std::multimap<float, std::string> > _jlrgr_index;

        ToBufferMgr<std::unordered_multimap<std::string, std::string, str_hasher> > _address_index;
        ToBufferMgr<std::unordered_multimap<std::string, std::string, str_hasher> > _plate_index;


        ToBufferMgr<std::vector<std::multimap<int, std::string> > > _rsingle_diff_index;

        //date, single, id
        ToBufferMgr<std::vector<std::map<std::string, std::multimap<int, std::string> > > > _hsingle_diff_index;

        //id, date
        ToBufferMgr<std::unordered_map<std::string, std::set<std::string>, str_hasher> > _hsid_date_index;

        // history quotation 
        ToBufferMgr<std::map<std::string, std::multimap<float, std::string> > > _hqend_index;
        ToBufferMgr<std::map<std::string, std::multimap<float, std::string> > > _hqchange_rate_index;
        ToBufferMgr<std::map<std::string, std::multimap<float, std::string> > > _hqrange_percent_index;

        ToBufferMgr<std::map<std::string, std::multimap<float, std::string> > > _hqdown_pointer_index;
        ToBufferMgr<std::map<std::string, std::multimap<float, std::string> > > _hqup_pointer_index;

        //recommend_flag, id
        ToBufferMgr<std::unordered_multimap<std::string, std::string, str_hasher> > _recommend_index;

    public:
        char proc_name[SIZE_LEN_256];

    private:
        static proc_data* _singleton;
};


#endif

