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
class trade_date_dict;
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

        reload_mgr<trade_date_dict> * _tdate_dict;

        sk_conf * _conf;


    public:
        //search index;
        ToBufferMgr<std::unordered_set<std::string, str_hasher> > _block_set;

        char proc_name[SIZE_LEN_256];

    private:
        static proc_data* _singleton;
};


#endif

