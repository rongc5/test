#ifndef __PROC_DATA_H__
#define __PROC_DATA_H__

#include "base_reload.h"
#include "sk_conf.h"
#include "finance_dict.h"

#include "address_search_index.h"
#include "hsingle_search_index.h"
#include "plate_search_index.h"
#include "rsingle_search_index.h"
#include "block_search_index.h"
#include "week_tradedate_search_index.h"
#include "hquotation_search_index.h"
#include "rquotation_search_index.h"
#include "finance_search_index.h"
#include "op_search_index.h"
#include "lruSsr_search_index.h"
#include "cir_holder_search_index.h"

class ua_dict;
class ban_dict;
class id_dict;
class addr_dict_split;
class addr_dict;
class plate_dict_split;
class plate_dict;
class cir_holder_dict_split;
class cir_holder_dict;
class history_single_dict;
class history_main_funds_dict;
class history_quotation_dict;
class history_wquotation_dict;
class history_wsingle_dict;
class holiday_dict;
class userid_dict;
class base_net_thread;
class proc_data:public reload_inf
{
    public:
        virtual ~proc_data();
        
        
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

        base_search_index get_search_index(string & key);

        void reg_handler();

        shared_ptr<url_handler> get_url_handler(string & key);

    public:
        reload_mgr<ua_dict> * _ua_dict;
        reload_mgr<id_dict> * _id_dict;

        reload_mgr<ban_dict> * _ban_dict;

        reload_mgr<addr_dict_split> * _addr_dict_split;

        reload_mgr<addr_dict> * _addr_dict;

        reload_mgr<plate_dict_split> * _plate_dict_split;

        reload_mgr<plate_dict> * _plate_dict;

        reload_mgr<cir_holder_dict_split> * _cir_holder_dict_split;

        reload_mgr<cir_holder_dict> * _cir_holder_dict;

        shared_ptr<history_single_dict> _hsingle_dict;

        shared_ptr<history_main_funds_dict> _hmain_funds_dict;

        shared_ptr<history_quotation_dict> _hquoation_dict;

        shared_ptr<history_wsingle_dict> _hwsingle_dict;

        shared_ptr<history_wquotation_dict> _hwquoation_dict;

        reload_mgr<holiday_dict> * _holiday_dict;

        shared_ptr<userid_dict> _userid_dict;

        sk_conf * _conf;

        finance_dict  _finance_dict;

    public:
        //search index;
        shared_ptr<block_search_index> _block_set;

        shared_ptr<week_tradedate_search_index> _wtdate_set;

        shared_ptr<address_search_index> _address_index;
        shared_ptr<plate_search_index> _plate_index;

        shared_ptr<cir_holder_search_index> _cir_holder_index;

        shared_ptr<rsingle_search_index> _rsingle_index;

        //date, single, id
        shared_ptr<hsingle_search_index> _hsingle_index;
        shared_ptr<hsingle_search_index> _hwsingle_index;

        // history quotation 
        shared_ptr<hquotation_search_index> _hquotation_index;

        shared_ptr<hquotation_search_index> _hwquotation_index;

        shared_ptr<finance_search_index> _finance_index;

        shared_ptr<rquotation_search_index> _rquotation_index;

        shared_ptr<op_search_index> _op_index;

        shared_ptr<lruSsr_search_index> _lrussr_index;

    public:

        void reg_search_sstr();

        int get_date_index(const string & id, string & date_index, string & date_index_end, search_res & search, set<int> & date_index_out, set<int> & date_index_end_out);
        int get_search_sstr(const string & id, const string & sstr, int date_index, int date_index_end, search_res & search, set<int> & res);
        void parse_sstr(const string & sstr, vector<string> & res);



        static int get_highest_index(const string & id, int date_index, int date_index_end, set<int> & res);
        static int get_lowest_index(const string & id, int date_index, int date_index_end, set<int> & res);

        static int get_trough_index(const string & id, int date_index, int date_index_end, set<int> & res);

        static int get_crest_index(const string & id, int date_index, int date_index_end, set<int> & res);

        void add_name_thread(const string & name, vector<base_net_thread *> & thread);
        void add_name_thread(const string & name, base_net_thread * thread);

        vector<base_net_thread *> * get_thread(const string & name);

        void update_req_date(const string & date);

        void update_trade_date(const string & date);

        const string * get_req_date();

        const string * get_trade_date();


    public:
        char proc_name[SIZE_LEN_256];

    private:
        map<string, base_search_index> _search_index_map;

        map<string, shared_ptr<url_handler> > _uhandler_map;

        map<string, search_sstr_index> _search_sstr_map;

        unordered_map<string, vector<base_net_thread *>, str_hasher> _name_thread_map;

        ToBufferMgr<string> _req_date;
        ToBufferMgr<string> _trade_date;

    private:
        static proc_data* _singleton;
};


#endif

