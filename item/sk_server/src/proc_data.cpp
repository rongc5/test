#include "log_helper.h"
#include "ua_dict.h"
#include "id_dict.h"
#include "finance_dict.h"
#include "ban_dict.h"
#include "proc_data.h"
#include "addr_dict_split.h"
#include "addr_dict.h"
#include "plate_dict_split.h"
#include "plate_dict.h"
#include "history_single_dict.h"
#include "history_quotation_dict.h"
#include "holiday_dict.h"
#include "userid_dict.h"
#include "uhandler_select.h"
#include "uhandler_queryid.h"
#include "uhandler_default.h"
#include "cir_holder_dict_split.h"
#include "cir_holder_dict.h"
//#include "history_wsingle_dict.h"
//#include "history_wquotation_dict.h"
#include "base_net_thread.h"
#include "history_main_funds_dict.h"

proc_data::~proc_data()
{
    curl_global_cleanup();
}

int proc_data::init(sk_conf * conf)
{
    get_proc_name(proc_name, sizeof(proc_name));

    reg_search_index();
    reg_handler();
    reg_search_sstr();

    CURLcode code = curl_global_init(CURL_GLOBAL_ALL);
    if (CURLE_OK != code)
    {
    LOG_WARNING("curl_global_init FAILED. CURLcode [%d]", code);
    return -1;
    }

    _conf = conf;
    
    {
        ua_dict *ua_dict1 = new (nothrow)ua_dict();
        ASSERT_WARNING(ua_dict1 != NULL, "allocate ua_dict fail");
        ua_dict1->init(_conf->_strategy->current()->ua_path.c_str(), 
                _conf->_strategy->current()->ua_file.c_str(), _conf->dump_dir.c_str());

        ua_dict *ua_dict2 = new (nothrow)ua_dict();
        ASSERT_WARNING(ua_dict2 != NULL, "allocate ua_dict fail");
        ua_dict2->init(_conf->_strategy->current()->ua_path.c_str(),
                _conf->_strategy->current()->ua_file.c_str(), _conf->dump_dir.c_str());

        _ua_dict = new (nothrow)reload_mgr<ua_dict>(ua_dict1, ua_dict2);
    }

    {
        id_dict *id_dict1 = new (nothrow)id_dict();
        ASSERT_WARNING(id_dict1 != NULL, "allocate id_dict fail");
        id_dict1->init(_conf->_strategy->current()->id_path.c_str(), 
                _conf->_strategy->current()->id_file.c_str(), _conf->dump_dir.c_str());

        id_dict *id_dict2 = new (nothrow)id_dict();
        ASSERT_WARNING(id_dict2 != NULL, "allocate id_dict fail");
        id_dict2->init(_conf->_strategy->current()->id_path.c_str(), 
                _conf->_strategy->current()->id_file.c_str(), _conf->dump_dir.c_str());

        _id_dict = new (nothrow)reload_mgr<id_dict>(id_dict1, id_dict2);
    }

    {
        addr_dict_split *addr_dict1 = new (nothrow)addr_dict_split();
        ASSERT_WARNING(addr_dict1 != NULL, "allocate addr_dict_split fail");
        addr_dict1->init(_conf->_strategy->current()->addr_dict_split_path.c_str(),
                _conf->_strategy->current()->addr_dict_split_file.c_str(), _conf->dump_dir.c_str());

        addr_dict_split *addr_dict2 = new (nothrow)addr_dict_split();
        ASSERT_WARNING(addr_dict2 != NULL, "allocate addr_dict_split fail");
        addr_dict2->init(_conf->_strategy->current()->addr_dict_split_path.c_str(),
                _conf->_strategy->current()->addr_dict_split_file.c_str(), _conf->dump_dir.c_str());

        _addr_dict_split = new (nothrow)reload_mgr<addr_dict_split>(addr_dict1, addr_dict2);
    }

    {
        addr_dict *addr_dict1 = new (nothrow)addr_dict();
        ASSERT_WARNING(addr_dict1 != NULL, "allocate addr_dict fail");
        addr_dict1->init(_conf->_strategy->current()->addr_dict_path.c_str(),
                _conf->_strategy->current()->addr_dict_file.c_str(), _conf->dump_dir.c_str());

        addr_dict *addr_dict2 = new (nothrow)addr_dict();
        ASSERT_WARNING(addr_dict2 != NULL, "allocate addr_dict fail");
        addr_dict2->init(_conf->_strategy->current()->addr_dict_path.c_str(),
                _conf->_strategy->current()->addr_dict_file.c_str(), _conf->dump_dir.c_str());

        _addr_dict = new (nothrow)reload_mgr<addr_dict>(addr_dict1, addr_dict2);
    }

    {
        plate_dict_split *addr_dict1 = new (nothrow)plate_dict_split();
        ASSERT_WARNING(addr_dict1 != NULL, "allocate plate_dict_split fail");
        addr_dict1->init(_conf->_strategy->current()->plate_dict_split_path.c_str(),
                _conf->_strategy->current()->plate_dict_split_file.c_str(), _conf->dump_dir.c_str());

        plate_dict_split *addr_dict2 = new (nothrow)plate_dict_split();
        ASSERT_WARNING(addr_dict2 != NULL, "allocate plate_dict_split fail");
        addr_dict2->init(_conf->_strategy->current()->plate_dict_split_path.c_str(),
                _conf->_strategy->current()->plate_dict_split_file.c_str(), _conf->dump_dir.c_str());

        _plate_dict_split = new (nothrow)reload_mgr<plate_dict_split>(addr_dict1, addr_dict2);
    }

    {
        plate_dict *addr_dict1 = new (nothrow)plate_dict();
        ASSERT_WARNING(addr_dict1 != NULL, "allocate plate_dict fail");
        addr_dict1->init(_conf->_strategy->current()->plate_dict_path.c_str(),
                _conf->_strategy->current()->plate_dict_file.c_str(), _conf->dump_dir.c_str());

        plate_dict *addr_dict2 = new (nothrow)plate_dict();
        ASSERT_WARNING(addr_dict2 != NULL, "allocate plate_dict fail");
        addr_dict2->init(_conf->_strategy->current()->plate_dict_path.c_str(),
                _conf->_strategy->current()->plate_dict_file.c_str(), _conf->dump_dir.c_str());

        _plate_dict = new (nothrow)reload_mgr<plate_dict>(addr_dict1, addr_dict2);
    }

    {
        cir_holder_dict_split *addr_dict1 = new (nothrow)cir_holder_dict_split();
        ASSERT_WARNING(addr_dict1 != NULL, "allocate cir_holder_dict_split fail");
        addr_dict1->init(_conf->_strategy->current()->cir_holder_dict_split_path.c_str(),
                _conf->_strategy->current()->cir_holder_dict_split_file.c_str(), _conf->dump_dir.c_str());

        cir_holder_dict_split *addr_dict2 = new (nothrow)cir_holder_dict_split();
        ASSERT_WARNING(addr_dict2 != NULL, "allocate cir_holder_dict_split fail");
        addr_dict2->init(_conf->_strategy->current()->cir_holder_dict_split_path.c_str(),
                _conf->_strategy->current()->cir_holder_dict_split_file.c_str(), _conf->dump_dir.c_str());

        _cir_holder_dict_split = new (nothrow)reload_mgr<cir_holder_dict_split>(addr_dict1, addr_dict2);
    }

    {
        cir_holder_dict *addr_dict1 = new (nothrow)cir_holder_dict();
        ASSERT_WARNING(addr_dict1 != NULL, "allocate cir_holder_dict fail");
        addr_dict1->init(_conf->_strategy->current()->cir_holder_dict_path.c_str(),
                _conf->_strategy->current()->cir_holder_dict_file.c_str(), _conf->dump_dir.c_str());

        cir_holder_dict *addr_dict2 = new (nothrow)cir_holder_dict();
        ASSERT_WARNING(addr_dict2 != NULL, "allocate cir_holder_dict fail");
        addr_dict2->init(_conf->_strategy->current()->cir_holder_dict_path.c_str(),
                _conf->_strategy->current()->cir_holder_dict_file.c_str(), _conf->dump_dir.c_str());

        _cir_holder_dict = new (nothrow)reload_mgr<cir_holder_dict>(addr_dict1, addr_dict2);
    }

    //id_rdict *id_dict1 = new (nothrow)id_rdict();
    //ASSERT_WARNING(id_dict1 != NULL, "allocate id_dict fail");
    //id_dict1->init(_conf->id_path.c_str(), _conf->id_file.c_str(), _conf->id_num, _conf->dump_dir.c_str());
    
    //id_rdict *id_dict2 = new (nothrow)id_rdict();
    //ASSERT_WARNING(id_dict2 != NULL, "allocate id_dict fail");
    //id_dict2->init(_conf->id_path.c_str(), _conf->id_file.c_str(), _conf->id_num, _conf->dump_dir.c_str());

    //_id_dict = new (nothrow)reload_mgr<id_rdict>(id_dict1, id_dict2);


    
    {
        _finance_dict.init(_conf->_strategy->current()->financie_path.c_str(),  
                conf->_strategy->current()->financie_file.c_str(), conf->dump_dir.c_str());
    }


    {
        ban_dict *b_dict1 = new (nothrow)ban_dict();
        ASSERT_WARNING(b_dict1 != NULL, "allocate ban_dict fail");
        b_dict1->init(_conf->_strategy->current()->ban_path.c_str(), 
                _conf->_strategy->current()->ban_file.c_str(), conf->dump_dir.c_str());

        ban_dict *b_dict2 = new (nothrow)ban_dict();
        ASSERT_WARNING(b_dict2 != NULL, "allocate ban_dict fail");
        b_dict2->init(_conf->_strategy->current()->ban_path.c_str(), 
                _conf->_strategy->current()->ban_file.c_str(), conf->dump_dir.c_str());

        _ban_dict = new (nothrow)reload_mgr<ban_dict>(b_dict1, b_dict2);
    }
    
    {
        _hsingle_dict = make_shared<history_single_dict>();
        _hsingle_dict->init(_conf->_strategy->current()->history_single_path.c_str(), 
                _conf->_strategy->current()->history_single_file.c_str(), conf->dump_dir.c_str());
    }
    {
        _hmain_funds_dict = make_shared<history_main_funds_dict>();
        _hmain_funds_dict->init(_conf->_strategy->current()->history_main_funds_path.c_str(), 
                _conf->_strategy->current()->history_main_funds_file.c_str(), conf->dump_dir.c_str());
    }


    {
        _hquoation_dict = make_shared<history_quotation_dict>();
        _hquoation_dict->init(_conf->_strategy->current()->history_quotation_path.c_str(),
                _conf->_strategy->current()->history_quotation_file.c_str(), conf->dump_dir.c_str());
    }

    {
        _userid_dict = make_shared<userid_dict>();
        _userid_dict->init(_conf->_strategy->current()->userid_dict_path.c_str(),
                _conf->_strategy->current()->userid_dict_file.c_str(), conf->dump_dir.c_str());
    }

    {
        holiday_dict *td_dict1 = new (nothrow)holiday_dict();
        ASSERT_WARNING(td_dict1 != NULL, "allocate holiday_dict fail");
        td_dict1->init(_conf->_strategy->current()->holiday_dict_path.c_str(), 
                _conf->_strategy->current()->holiday_dict_file.c_str(), conf->dump_dir.c_str());

        holiday_dict *td_dict2 = new (nothrow)holiday_dict();
        ASSERT_WARNING(td_dict2 != NULL, "allocate holiday_dict fail");
        td_dict2->init(_conf->_strategy->current()->holiday_dict_path.c_str(),
                _conf->_strategy->current()->holiday_dict_file.c_str(), conf->dump_dir.c_str());

        _holiday_dict = new (nothrow)reload_mgr<holiday_dict>(td_dict1, td_dict2);

    }

//    {
//        _hwsingle_dict = make_shared<history_wsingle_dict>();
//        _hwsingle_dict->init(_conf->_strategy->current()->history_wsingle_path.c_str(),
//                _conf->_strategy->current()->history_wsingle_file.c_str(), conf->dump_dir.c_str());
//    }


//    {
//        _hwquoation_dict = make_shared<history_wquotation_dict>();
//        _hwquoation_dict->init(_conf->_strategy->current()->history_wquotation_path.c_str(),
//                _conf->_strategy->current()->history_wquotation_file.c_str(), conf->dump_dir.c_str());
//    }

    return 0;
}

void proc_data::add_name_thread(const string & name, base_net_thread * thread)
{

    auto ii = _name_thread_map.find(name);
    if (ii != _name_thread_map.end())
    {
        ii->second.push_back(thread);
    }
    else
    {
        vector<base_net_thread *> vc;
        vc.push_back(thread);
        _name_thread_map.insert(make_pair(name, vc));
    }
}

vector<base_net_thread *> * proc_data::get_thread(const string & name)
{
    auto ii = _name_thread_map.find(name);
    if (ii != _name_thread_map.end())
    {
        return &(ii->second);
    }

    return NULL;
}

void proc_data::add_name_thread(const string & name, vector<base_net_thread *> & thread)
{
    _name_thread_map.insert(make_pair(name, thread));
}


proc_data* proc_data::_singleton;

proc_data * proc_data::instance()
{
    if(!_singleton) 
    {
        _singleton = new(nothrow)proc_data();
        if(_singleton == NULL)
        {
            LOG_WARNING("new Proc_data fail");
            exit(1);
        }
    }

    return _singleton;
}

int proc_data::load()
{
    _ua_dict->load();

    _id_dict->load();

    _finance_dict.load();

    _ban_dict->load();

    _addr_dict_split->load();

    _addr_dict->load();

    _plate_dict_split->load();

    _plate_dict->load();

    _cir_holder_dict_split->load();

    _cir_holder_dict->load();

    _hsingle_dict->load();
    
    _hmain_funds_dict->load();

    _hquoation_dict->load();

    _userid_dict->load();

//    _hwsingle_dict->load();

//    _hwquoation_dict->load();

    _holiday_dict->load();

    return 0;
}
int proc_data::reload()
{
    int flag = 0;
    if (_ua_dict && _ua_dict->need_reload())
    {
        _ua_dict->reload();
        flag = 1;
    }

    if (_id_dict && _id_dict->need_reload())
    {
        _id_dict->reload();
        flag = 1;
    }

    if (_finance_dict.need_reload())
    {
        _finance_dict.reload();
        flag = 1;
    }

    if (_ban_dict && _ban_dict->need_reload())
    {
        _ban_dict->reload();
        flag = 1;
    }

    if (_addr_dict_split && _addr_dict_split->need_reload())
    {
        _addr_dict_split->reload();
        flag = 1;
    }

    if (_addr_dict && _addr_dict->need_reload())
    {
        _addr_dict->reload();
        flag = 1;
    }

    if (_plate_dict_split && _plate_dict_split->need_reload())
    {
        _plate_dict_split->reload();
        flag = 1;
    }

    if (_plate_dict && _plate_dict->need_reload())
    {
        _plate_dict->reload();
        flag = 1;
    }

    if (_cir_holder_dict_split && _cir_holder_dict_split->need_reload())
    {
        _cir_holder_dict_split->reload();
        flag = 1;
    }

    if (_cir_holder_dict && _cir_holder_dict->need_reload())
    {
        _cir_holder_dict->reload();
        flag = 1;
    }


    if (_hsingle_dict && _hsingle_dict->need_reload())
    {
        _hsingle_dict->reload();
        flag = 1;
    }
    if (_hmain_funds_dict && _hmain_funds_dict->need_reload())
    {
        _hmain_funds_dict->reload();
        flag = 1;
    }

    if (_hquoation_dict && _hquoation_dict->need_reload())
    {
        _hquoation_dict->reload();
        flag = 1;
    }

    if (_userid_dict && _userid_dict->need_reload())
    {
        _userid_dict->reload();
        flag = 1;
    }

//    if (_hwsingle_dict && _hwsingle_dict->need_reload())
//    {
//        _hwsingle_dict->reload();
//        flag = 1;
//    }

//    if (_hwquoation_dict && _hwquoation_dict->need_reload())
//    {
//        _hwquoation_dict->reload();
//        flag = 1;
//    }

    if (_holiday_dict && _holiday_dict->need_reload())
    {
        _holiday_dict->reload();
        flag = 1;
    }

    return flag;
}
bool proc_data::need_reload()
{
    return true;
}
int proc_data::dump()
{
    _ua_dict->dump();

    _id_dict->dump();

    _finance_dict.dump();

    _ban_dict->dump();

    _addr_dict_split->dump();

    _addr_dict->dump();

    _plate_dict_split->dump();

    _plate_dict->dump();

    _cir_holder_dict_split->dump();

    _cir_holder_dict->dump();

    _hsingle_dict->dump();

    _hmain_funds_dict->dump();

    _hquoation_dict->dump();

    _userid_dict->dump();

//    _hwsingle_dict->dump();

//    _hwquoation_dict->dump();

    _holiday_dict->dump();

    return 0;
}
int proc_data::destroy()
{
    _ua_dict->destroy();

    _id_dict->destroy();

    _finance_dict.destroy();

    _ban_dict->destroy();

    _addr_dict_split->destroy();

    _addr_dict->destroy();

    _plate_dict_split->destroy();

    _plate_dict->destroy();

    _cir_holder_dict_split->destroy();

    _cir_holder_dict->destroy();

    _hsingle_dict->destroy();

    _hmain_funds_dict->destroy();

    _hquoation_dict->destroy();

    _userid_dict->destroy();

//    _hwsingle_dict->destroy();

//    _hwquoation_dict->destroy();

    _holiday_dict->destroy();

    return 0;
}

int proc_data::destroy_idle()
{
    _ua_dict->idle()->destroy();

    _id_dict->idle()->destroy();

    _finance_dict.destroy();

    _ban_dict->idle()->destroy();

    _addr_dict_split->idle()->destroy();

    _addr_dict->idle()->destroy();

    _plate_dict_split->idle()->destroy();

    _plate_dict->idle()->destroy();

    _cir_holder_dict_split->idle()->destroy();

    _cir_holder_dict->idle()->destroy();

    // _hsingle_dict->idle()->destroy();

    // _hquoation_dict->idle()->destroy();

    // _hmain_funds_dict->idle()->destroy();
    //_hwsingle_dict->idle()->destroy();

    //_hwquoation_dict->idle()->destroy();

    _holiday_dict->idle()->destroy();

    return 0;
}

void proc_data::reg_search_index()
{
    _lrussr_index = make_shared<lruSsr_search_index>();
    _wtdate_set = make_shared<week_tradedate_search_index>();

    _op_index = make_shared<op_search_index>();
    _search_index_map["op"] = bind(&op_search_index::do_check_op_search, _op_index, _1, _2, _3);

    _block_set = make_shared<block_search_index>();
    _search_index_map["block"] = bind(&block_search_index::do_check_block_search, _block_set, _1, _2, _3);

    _rmain_funds_index = make_shared<rmain_funds_search_index>();

    _rquotation_index = make_shared<rquotation_search_index>();
    _search_index_map["end_le"] = bind(&rquotation_search_index::do_check_end_le, _rquotation_index, _1, _2, _3);
    _search_index_map["end_ge"] = bind(&rquotation_search_index::do_check_end_ge, _rquotation_index, _1, _2, _3);

    _search_index_map["change_rate_le"] = bind(&rquotation_search_index::do_check_change_rate_le, _rquotation_index, _1, _2, _3);
    _search_index_map["change_rate_ge"] = bind(&rquotation_search_index::do_check_change_rate_ge, _rquotation_index, _1, _2, _3);

    _search_index_map["range_percent_le"] = bind(&rquotation_search_index::do_check_range_percent_le, _rquotation_index, _1, _2, _3);
    _search_index_map["range_percent_ge"] = bind(&rquotation_search_index::do_check_range_percent_ge, _rquotation_index, _1, _2, _3);

    _search_index_map["down_pointer_le"] = bind(&rquotation_search_index::do_check_down_pointer_le, _rquotation_index, _1, _2, _3);
    _search_index_map["down_pointer_ge"] = bind(&rquotation_search_index::do_check_down_pointer_ge, _rquotation_index, _1, _2, _3);

    _search_index_map["up_pointer_le"] = bind(&rquotation_search_index::do_check_up_pointer_le, _rquotation_index, _1, _2, _3);
    _search_index_map["up_pointer_ge"] = bind(&rquotation_search_index::do_check_up_pointer_ge, _rquotation_index, _1, _2, _3);

    _search_index_map["end_avg_end_le"] = bind(&rquotation_search_index::do_check_end_avg_end_le, _rquotation_index, _1, _2, _3);
    _search_index_map["end_avg_end_ge"] = bind(&rquotation_search_index::do_check_end_avg_end_ge, _rquotation_index, _1, _2, _3);

    _search_index_map["end_end5_le"] = bind(&rquotation_search_index::do_check_end_end5_le, _rquotation_index, _1, _2, _3);
    _search_index_map["end_end5_ge"] = bind(&rquotation_search_index::do_check_end_end5_ge, _rquotation_index, _1, _2, _3);

    _search_index_map["end_end10_le"] = bind(&rquotation_search_index::do_check_end_end10_le, _rquotation_index, _1, _2, _3);
    _search_index_map["end_end10_ge"] = bind(&rquotation_search_index::do_check_end_end10_ge, _rquotation_index, _1, _2, _3);

    _search_index_map["end_end20_le"] = bind(&rquotation_search_index::do_check_end_end20_le, _rquotation_index, _1, _2, _3);
    _search_index_map["end_end20_ge"] = bind(&rquotation_search_index::do_check_end_end20_ge, _rquotation_index, _1, _2, _3);

    /*
    _search_index_map["end_end30_le"] = bind(&rquotation_search_index::do_check_end_end30_le, _rquotation_index, _1, _2, _3);
    _search_index_map["end_end30_ge"] = bind(&rquotation_search_index::do_check_end_end30_ge, _rquotation_index, _1, _2, _3);
    */

    _finance_index = make_shared<finance_search_index>();
    _search_index_map["pe_le"] = bind(&finance_search_index::do_check_pe_le, _finance_index, _1, _2, _3);
    _search_index_map["pe_ge"] = bind(&finance_search_index::do_check_pe_ge, _finance_index, _1, _2, _3);

    _search_index_map["pb_le"] = bind(&finance_search_index::do_check_pb_le, _finance_index, _1, _2, _3);
    _search_index_map["pb_ge"] = bind(&finance_search_index::do_check_pb_ge, _finance_index, _1, _2, _3);

    _search_index_map["value_le"] = bind(&finance_search_index::do_check_value_le, _finance_index, _1, _2, _3);
    _search_index_map["value_ge"] = bind(&finance_search_index::do_check_value_ge, _finance_index, _1, _2, _3);

    _search_index_map["cir_value_le"] = bind(&finance_search_index::do_check_cir_value_le, _finance_index, _1, _2, _3);
    _search_index_map["cir_value_ge"] = bind(&finance_search_index::do_check_cir_value_ge, _finance_index, _1, _2, _3);

    _search_index_map["mgxj_le"] = bind(&finance_search_index::do_check_mgxj_le, _finance_index, _1, _2, _3);
    _search_index_map["mgxj_ge"] = bind(&finance_search_index::do_check_mgxj_ge, _finance_index, _1, _2, _3);

    _search_index_map["mgsy_le"] = bind(&finance_search_index::do_check_mgsy_le, _finance_index, _1, _2, _3);
    _search_index_map["mgsy_ge"] = bind(&finance_search_index::do_check_mgsy_ge, _finance_index, _1, _2, _3);

    _search_index_map["zysrgr_le"] = bind(&finance_search_index::do_check_zysrgr_le, _finance_index, _1, _2, _3);
    _search_index_map["zysrgr_ge"] = bind(&finance_search_index::do_check_zysrgr_ge, _finance_index, _1, _2, _3);

    _search_index_map["jlrgr_le"] = bind(&finance_search_index::do_check_jlrgr_le, _finance_index, _1, _2, _3);
    _search_index_map["jlrgr_ge"] = bind(&finance_search_index::do_check_jlrgr_ge, _finance_index, _1, _2, _3);

    _address_index = make_shared<address_search_index>();
    _search_index_map["address"] = bind(&address_search_index::search, _address_index, _1, _2, _3);

    _plate_index = make_shared<plate_search_index>();
    _search_index_map["plate"] = bind(&plate_search_index::search, _plate_index, _1, _2, _3);

    _cir_holder_index = make_shared<cir_holder_search_index>();
    _search_index_map["cir_holder"] = bind(&cir_holder_search_index::search, _cir_holder_index, _1, _2, _3);

    _rsingle_index = make_shared<rsingle_search_index>();
    _search_index_map["rsingle_diff_ge"] = bind(&rsingle_search_index::do_check_rsingle_ge, _rsingle_index, _1, _2, _3);
    _search_index_map["rsingle_diff_le"] = bind(&rsingle_search_index::do_check_rsingle_le, _rsingle_index, _1, _2, _3);

    _search_index_map["rsingle_diff2_ge"] = bind(&rsingle_search_index::do_check_rsingle_diff2_ge, _rsingle_index, _1, _2, _3);
    _search_index_map["rsingle_diff2_le"] = bind(&rsingle_search_index::do_check_rsingle_diff2_le, _rsingle_index, _1, _2, _3);

    _hsingle_index = make_shared<hsingle_search_index>();
    _search_index_map["hsingle_diff_ge"] = bind(&hsingle_search_index::do_hsingle_diff_ge, _hsingle_index, _1, _2, _3);
    _search_index_map["hsingle_diff_le"] = bind(&hsingle_search_index::do_hsingle_diff_le, _hsingle_index, _1, _2, _3);

    _search_index_map["hsingle_diff2_ge"] = bind(&hsingle_search_index::do_hsingle_diff2_ge, _hsingle_index, _1, _2, _3);
    _search_index_map["hsingle_diff2_le"] = bind(&hsingle_search_index::do_hsingle_diff2_le, _hsingle_index, _1, _2, _3);

    _search_index_map["hsingle_diff_ge_num_ge"] = bind(&hsingle_search_index::do_hsingle_diff_ge_num_ge, _hsingle_index, _1, _2, _3);
    _search_index_map["hsingle_diff_sum_ge"] = bind(&hsingle_search_index::do_hsingle_sum_ge, _hsingle_index, _1, _2, _3);
    _search_index_map["hsingle_diff_sum_le"] = bind(&hsingle_search_index::do_hsingle_sum_le, _hsingle_index, _1, _2, _3);


    _search_index_map["hsratio_ge"] = bind(&hsingle_search_index::do_hsratio_ge, _hsingle_index, _1, _2, _3);
    _search_index_map["hsratio_le"] = bind(&hsingle_search_index::do_hsratio_le, _hsingle_index, _1, _2, _3);
    _search_index_map["hsratio_ge_num_ge"] = bind(&hsingle_search_index::do_hsratio_ge_num_ge, _hsingle_index, _1, _2, _3);
    _search_index_map["hsratio_sum_ge"] = bind(&hsingle_search_index::do_hsingle_sum_ge, _hsingle_index, _1, _2, _3);
    _search_index_map["hsratio_sum_le"] = bind(&hsingle_search_index::do_hsingle_sum_le, _hsingle_index, _1, _2, _3);

    _hmain_funds_index = make_shared<hmain_funds_search_index>();

    _hquotation_index = make_shared<hquotation_search_index>();

    _search_index_map["hqchange_rate_ge"] = bind(&hquotation_search_index::do_hqchange_rate_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["hqchange_rate_le"] = bind(&hquotation_search_index::do_hqchange_rate_le, _hquotation_index, _1, _2, _3);

    _search_index_map["hqrange_percent_ge"] = bind(&hquotation_search_index::do_hqrange_percent_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["hqrange_percent_le"] = bind(&hquotation_search_index::do_hqrange_percent_le, _hquotation_index, _1, _2, _3);

    _search_index_map["hqrange_percent_ge_num_ge"] = bind(&hquotation_search_index::do_hqrange_percent_ge_num_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["hqrange_percent_le_num_ge"] = bind(&hquotation_search_index::do_hqrange_percent_le_num_ge, _hquotation_index, _1, _2, _3);

    _search_index_map["hq_sum_range_percent_ge"] = bind(&hquotation_search_index::do_hq_sum_range_percent_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["hq_sum_range_percent_le"] = bind(&hquotation_search_index::do_hq_sum_range_percent_le, _hquotation_index, _1, _2, _3);

    _search_index_map["hq_sum_change_rate_ge"] = bind(&hquotation_search_index::do_hq_sum_change_rate_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["hq_sum_change_rate_le"] = bind(&hquotation_search_index::do_hq_sum_change_rate_le, _hquotation_index, _1, _2, _3);

    _search_index_map["hqdown_pointer_ge"] = bind(&hquotation_search_index::do_check_hqdown_pointer_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["hqdown_pointer_le"] = bind(&hquotation_search_index::do_check_hqdown_pointer_le, _hquotation_index, _1, _2, _3);
    _search_index_map["hqdown_pointer_ge_num_ge"] = bind(&hquotation_search_index::do_check_hqdown_pointer_ge_num_ge, _hquotation_index, _1, _2, _3);

    _search_index_map["hqup_pointer_ge"] = bind(&hquotation_search_index::do_check_hqup_pointer_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["hqup_pointer_le"] = bind(&hquotation_search_index::do_check_hqup_pointer_le, _hquotation_index, _1, _2, _3);
    _search_index_map["hqup_pointer_ge_num_ge"] = bind(&hquotation_search_index::do_check_hqup_pointer_ge_num_ge, _hquotation_index, _1, _2, _3);

    _search_index_map["hqend_start_ge"] = bind(&hquotation_search_index::do_check_hqend_start_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["hqend_start_le"] = bind(&hquotation_search_index::do_check_hqend_start_le, _hquotation_index, _1, _2, _3);
    _search_index_map["hqend_start_ge_num_ge"] = bind(&hquotation_search_index::do_check_hqend_start_ge_num_ge, _hquotation_index, _1, _2, _3);

    _search_index_map["hqend_start_ge_ratio_ge"] = bind(&hquotation_search_index::do_check_hqend_start_ge_ratio_ge, _hquotation_index, _1, _2, _3);

    _search_index_map["hqend_avg_end_ge"] = bind(&hquotation_search_index::do_check_hqend_avg_end_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["hqend_avg_end_le"] = bind(&hquotation_search_index::do_check_hqend_avg_end_le, _hquotation_index, _1, _2, _3);
    _search_index_map["hqend_avg_end_ge_num_ge"] = bind(&hquotation_search_index::do_check_hqend_avg_end_ge_num_ge, _hquotation_index, _1, _2, _3);

    _search_index_map["id_substr"] = bind(&hquotation_search_index::do_check_id_substr, _hquotation_index, _1, _2, _3);


    _search_index_map["rlow_hlow_ge"] = bind(&hquotation_search_index::do_check_rlow_hlow_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["rlow_hlow_le"] = bind(&hquotation_search_index::do_check_rlow_hlow_le, _hquotation_index, _1, _2, _3);
    _search_index_map["rlow_hlow_ge_num_ge"] = bind(&hquotation_search_index::do_check_rlow_hlow_ge_num_ge, _hquotation_index, _1, _2, _3);

    _search_index_map["rlow_hlowest_ge"] = bind(&hquotation_search_index::do_check_rlow_hlowest_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["rlow_hlowest_le"] = bind(&hquotation_search_index::do_check_rlow_hlowest_le, _hquotation_index, _1, _2, _3);

    _search_index_map["rlow_htrough_ge"] = bind(&hquotation_search_index::do_check_rlow_htrough_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["rlow_htrough_le"] = bind(&hquotation_search_index::do_check_rlow_htrough_le, _hquotation_index, _1, _2, _3);

    _search_index_map["rhigh_hhigh_ge"] = bind(&hquotation_search_index::do_check_rhigh_hhigh_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["rhigh_hhigh_le"] = bind(&hquotation_search_index::do_check_rhigh_hhigh_le, _hquotation_index, _1, _2, _3);
    _search_index_map["rhigh_hhigh_ge_num_ge"] = bind(&hquotation_search_index::do_check_rhigh_hhigh_ge_num_ge, _hquotation_index, _1, _2, _3);

    _search_index_map["rhigh_hhighest_ge"] = bind(&hquotation_search_index::do_check_rhigh_hhighest_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["rhigh_hhighest_le"] = bind(&hquotation_search_index::do_check_rhigh_hhighest_le, _hquotation_index, _1, _2, _3);

    _search_index_map["rhigh_hcrest_ge"] = bind(&hquotation_search_index::do_check_rhigh_hcrest_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["rhigh_hcrest_le"] = bind(&hquotation_search_index::do_check_rhigh_hcrest_le, _hquotation_index, _1, _2, _3);

    _search_index_map["hqend_end_5_ge"] = bind(&hquotation_search_index::do_check_hqend_end_5_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["hqend_end_5_le"] = bind(&hquotation_search_index::do_check_hqend_end_5_le, _hquotation_index, _1, _2, _3);
    _search_index_map["hqend_end_5_ge_num_ge"] = bind(&hquotation_search_index::do_check_hqend_end_5_ge_num_ge, _hquotation_index, _1, _2, _3);

    _search_index_map["hqend_end_10_ge"] = bind(&hquotation_search_index::do_check_hqend_end_10_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["hqend_end_10_le"] = bind(&hquotation_search_index::do_check_hqend_end_10_le, _hquotation_index, _1, _2, _3);
    _search_index_map["hqend_end_10_ge_num_ge"] = bind(&hquotation_search_index::do_check_hqend_end_10_ge_num_ge, _hquotation_index, _1, _2, _3);

    _search_index_map["hqend_end_20_ge"] = bind(&hquotation_search_index::do_check_hqend_end_20_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["hqend_end_20_le"] = bind(&hquotation_search_index::do_check_hqend_end_20_le, _hquotation_index, _1, _2, _3);
    _search_index_map["hqend_end_20_ge_num_ge"] = bind(&hquotation_search_index::do_check_hqend_end_20_ge_num_ge, _hquotation_index, _1, _2, _3);

    /*
    _search_index_map["hqend_end_30_ge"] = bind(&hquotation_search_index::do_check_hqend_end_30_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["hqend_end_30_le"] = bind(&hquotation_search_index::do_check_hqend_end_30_le, _hquotation_index, _1, _2, _3);
    _search_index_map["hqend_end_30_ge_num_ge"] = bind(&hquotation_search_index::do_check_hqend_end_30_ge_num_ge, _hquotation_index, _1, _2, _3);
    */

    _search_index_map["hqredvol_greenvol_ge"] = bind(&hquotation_search_index::do_check_hqredvol_greenvol_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["hqredvol_greenvol_le"] = bind(&hquotation_search_index::do_check_hqredvol_greenvol_le, _hquotation_index, _1, _2, _3);

    _search_index_map["hqend_end_le"] = bind(&hquotation_search_index::do_check_hqend_end_le, _hquotation_index, _1, _2, _3);
    _search_index_map["hqend_end_ge"] = bind(&hquotation_search_index::do_check_hqend_end_ge, _hquotation_index, _1, _2, _3);

    _search_index_map["hqswing_le"] = bind(&hquotation_search_index::do_check_hqswing_le, _hquotation_index, _1, _2, _3);
    _search_index_map["hqswing_ge"] = bind(&hquotation_search_index::do_check_hqswing_ge, _hquotation_index, _1, _2, _3);

    _search_index_map["hqlow_end_5_ge"] = bind(&hquotation_search_index::do_check_hqlow_end_5_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["hqlow_end_5_le"] = bind(&hquotation_search_index::do_check_hqlow_end_5_le, _hquotation_index, _1, _2, _3);

    _search_index_map["hqhigh_end_5_ge"] = bind(&hquotation_search_index::do_check_hqhigh_end_5_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["hqhigh_end_5_le"] = bind(&hquotation_search_index::do_check_hqhigh_end_5_le, _hquotation_index, _1, _2, _3);

#if 0
    _hwquotation_index = make_shared<hquotation_search_index>();

    _search_index_map["hwqchange_rate_ge"] = bind(&hquotation_search_index::do_hqchange_rate_ge, _hwquotation_index, _1, _2, _3);
    _search_index_map["hwqchange_rate_le"] = bind(&hquotation_search_index::do_hqchange_rate_le, _hwquotation_index, _1, _2, _3);

    _search_index_map["hwqrange_percent_ge"] = bind(&hquotation_search_index::do_hqrange_percent_ge, _hwquotation_index, _1, _2, _3);
    _search_index_map["hwqrange_percent_le"] = bind(&hquotation_search_index::do_hqrange_percent_le, _hwquotation_index, _1, _2, _3);

    _search_index_map["hwqrange_percent_ge_num_ge"] = bind(&hquotation_search_index::do_hqrange_percent_ge_num_ge, _hwquotation_index, _1, _2, _3);
    _search_index_map["hwqrange_percent_le_num_ge"] = bind(&hquotation_search_index::do_hqrange_percent_le_num_ge, _hwquotation_index, _1, _2, _3);

    _search_index_map["hwq_sum_range_percent_ge"] = bind(&hquotation_search_index::do_hq_sum_range_percent_ge, _hwquotation_index, _1, _2, _3);
    _search_index_map["hwq_sum_range_percent_le"] = bind(&hquotation_search_index::do_hq_sum_range_percent_le, _hwquotation_index, _1, _2, _3);

    _search_index_map["hwq_sum_change_rate_ge"] = bind(&hquotation_search_index::do_hq_sum_change_rate_ge, _hwquotation_index, _1, _2, _3);
    _search_index_map["hwq_sum_change_rate_le"] = bind(&hquotation_search_index::do_hq_sum_change_rate_le, _hwquotation_index, _1, _2, _3);

    _search_index_map["hwqdown_pointer_ge"] = bind(&hquotation_search_index::do_check_hqdown_pointer_ge, _hwquotation_index, _1, _2, _3);
    _search_index_map["hwqdown_pointer_le"] = bind(&hquotation_search_index::do_check_hqdown_pointer_le, _hwquotation_index, _1, _2, _3);
    _search_index_map["hwqdown_pointer_ge_num_ge"] = bind(&hquotation_search_index::do_check_hqdown_pointer_ge_num_ge, _hwquotation_index, _1, _2, _3);

    _search_index_map["hwqup_pointer_ge"] = bind(&hquotation_search_index::do_check_hqup_pointer_ge, _hwquotation_index, _1, _2, _3);
    _search_index_map["hwqup_pointer_le"] = bind(&hquotation_search_index::do_check_hqup_pointer_le, _hwquotation_index, _1, _2, _3);
    _search_index_map["hwqup_pointer_ge_num_ge"] = bind(&hquotation_search_index::do_check_hqup_pointer_ge_num_ge, _hwquotation_index, _1, _2, _3);

    _search_index_map["hwqend_start_ge"] = bind(&hquotation_search_index::do_check_hqend_start_ge, _hwquotation_index, _1, _2, _3);
    _search_index_map["hwqend_start_le"] = bind(&hquotation_search_index::do_check_hqend_start_le, _hwquotation_index, _1, _2, _3);
    _search_index_map["hwqend_start_ge_num_ge"] = bind(&hquotation_search_index::do_check_hqend_start_ge_num_ge, _hwquotation_index, _1, _2, _3);

    _search_index_map["hwqend_start_ge_ratio_ge"] = bind(&hquotation_search_index::do_check_hqend_start_ge_ratio_ge, _hwquotation_index, _1, _2, _3);

    _search_index_map["hwqend_avg_end_ge"] = bind(&hquotation_search_index::do_check_hqend_avg_end_ge, _hwquotation_index, _1, _2, _3);
    _search_index_map["hwqend_avg_end_le"] = bind(&hquotation_search_index::do_check_hqend_avg_end_le, _hwquotation_index, _1, _2, _3);
    _search_index_map["hwqend_avg_end_ge_num_ge"] = bind(&hquotation_search_index::do_check_hqend_avg_end_ge_num_ge, _hwquotation_index, _1, _2, _3);

    _search_index_map["rlow_hwlow_ge"] = bind(&hquotation_search_index::do_check_rlow_hlow_ge, _hwquotation_index, _1, _2, _3);
    _search_index_map["rlow_hwlow_le"] = bind(&hquotation_search_index::do_check_rlow_hlow_le, _hwquotation_index, _1, _2, _3);
    _search_index_map["rlow_hwlow_ge_num_ge"] = bind(&hquotation_search_index::do_check_rlow_hlow_ge_num_ge, _hwquotation_index, _1, _2, _3);

    _search_index_map["rlow_hwlowest_ge"] = bind(&hquotation_search_index::do_check_rlow_hlowest_ge, _hwquotation_index, _1, _2, _3);
    _search_index_map["rlow_hwlowest_le"] = bind(&hquotation_search_index::do_check_rlow_hlowest_le, _hwquotation_index, _1, _2, _3);

    _search_index_map["rlow_hwtrough_ge"] = bind(&hquotation_search_index::do_check_rlow_htrough_ge, _hwquotation_index, _1, _2, _3);
    _search_index_map["rlow_hwtrough_le"] = bind(&hquotation_search_index::do_check_rlow_htrough_le, _hwquotation_index, _1, _2, _3);

    _search_index_map["rhigh_hwhigh_ge"] = bind(&hquotation_search_index::do_check_rhigh_hhigh_ge, _hwquotation_index, _1, _2, _3);
    _search_index_map["rhigh_hwhigh_le"] = bind(&hquotation_search_index::do_check_rhigh_hhigh_le, _hwquotation_index, _1, _2, _3);
    _search_index_map["rhigh_hwhigh_ge_num_ge"] = bind(&hquotation_search_index::do_check_rhigh_hhigh_ge_num_ge, _hwquotation_index, _1, _2, _3);

    _search_index_map["rhigh_hwhighest_ge"] = bind(&hquotation_search_index::do_check_rhigh_hhighest_ge, _hwquotation_index, _1, _2, _3);
    _search_index_map["rhigh_hwhighest_le"] = bind(&hquotation_search_index::do_check_rhigh_hhighest_le, _hwquotation_index, _1, _2, _3);

    _search_index_map["rhigh_hwcrest_ge"] = bind(&hquotation_search_index::do_check_rhigh_hcrest_ge, _hwquotation_index, _1, _2, _3);
    _search_index_map["rhigh_hwcrest_le"] = bind(&hquotation_search_index::do_check_rhigh_hcrest_le, _hwquotation_index, _1, _2, _3);

    _search_index_map["hwqend_end_5_ge"] = bind(&hquotation_search_index::do_check_hqend_end_5_ge, _hwquotation_index, _1, _2, _3);
    _search_index_map["hwqend_end_5_le"] = bind(&hquotation_search_index::do_check_hqend_end_5_le, _hwquotation_index, _1, _2, _3);
    _search_index_map["hwqend_end_5_ge_num_ge"] = bind(&hquotation_search_index::do_check_hqend_end_5_ge_num_ge, _hwquotation_index, _1, _2, _3);

    _search_index_map["hwqend_end_10_ge"] = bind(&hquotation_search_index::do_check_hqend_end_10_ge, _hwquotation_index, _1, _2, _3);
    _search_index_map["hwqend_end_10_le"] = bind(&hquotation_search_index::do_check_hqend_end_10_le, _hwquotation_index, _1, _2, _3);
    _search_index_map["hwqend_end_10_ge_num_ge"] = bind(&hquotation_search_index::do_check_hqend_end_10_ge_num_ge, _hwquotation_index, _1, _2, _3);

    _search_index_map["hwqend_end_20_ge"] = bind(&hquotation_search_index::do_check_hqend_end_20_ge, _hwquotation_index, _1, _2, _3);
    _search_index_map["hwqend_end_20_le"] = bind(&hquotation_search_index::do_check_hqend_end_20_le, _hwquotation_index, _1, _2, _3);
    _search_index_map["hwqend_end_20_ge_num_ge"] = bind(&hquotation_search_index::do_check_hqend_end_20_ge_num_ge, _hwquotation_index, _1, _2, _3);

    /*
    _search_index_map["hwqend_end_30_ge"] = bind(&hquotation_search_index::do_check_hqend_end_30_ge, _hwquotation_index, _1, _2, _3);
    _search_index_map["hwqend_end_30_le"] = bind(&hquotation_search_index::do_check_hqend_end_30_le, _hwquotation_index, _1, _2, _3);
    _search_index_map["hwqend_end_30_ge_num_ge"] = bind(&hquotation_search_index::do_check_hqend_end_30_ge_num_ge, _hwquotation_index, _1, _2, _3);
    */

    _search_index_map["hwqredvol_greenvol_ge"] = bind(&hquotation_search_index::do_check_hqredvol_greenvol_ge, _hwquotation_index, _1, _2, _3);
    _search_index_map["hwqredvol_greenvol_le"] = bind(&hquotation_search_index::do_check_hqredvol_greenvol_le, _hwquotation_index, _1, _2, _3);

    _search_index_map["hwqend_end_le"] = bind(&hquotation_search_index::do_check_hqend_end_le, _hwquotation_index, _1, _2, _3);
    _search_index_map["hwqend_end_ge"] = bind(&hquotation_search_index::do_check_hqend_end_ge, _hwquotation_index, _1, _2, _3);

    _search_index_map["hwqswing_le"] = bind(&hquotation_search_index::do_check_hqswing_le, _hwquotation_index, _1, _2, _3);
    _search_index_map["hwqswing_ge"] = bind(&hquotation_search_index::do_check_hqswing_ge, _hwquotation_index, _1, _2, _3);

    _search_index_map["hwqlow_end_5_ge"] = bind(&hquotation_search_index::do_check_hqlow_end_5_ge, _hwquotation_index, _1, _2, _3);
    _search_index_map["hwqlow_end_5_le"] = bind(&hquotation_search_index::do_check_hqlow_end_5_le, _hwquotation_index, _1, _2, _3);

    _search_index_map["hwqhigh_end_5_ge"] = bind(&hquotation_search_index::do_check_hqhigh_end_5_ge, _hwquotation_index, _1, _2, _3);
    _search_index_map["hwqhigh_end_5_le"] = bind(&hquotation_search_index::do_check_hqhigh_end_5_le, _hwquotation_index, _1, _2, _3);


    _hwsingle_index = make_shared<hsingle_search_index>();
    _search_index_map["hwsingle_diff_ge"] = bind(&hsingle_search_index::do_hsingle_diff_ge, _hwsingle_index, _1, _2, _3);
    _search_index_map["hwsingle_diff_le"] = bind(&hsingle_search_index::do_hsingle_diff_le, _hwsingle_index, _1, _2, _3);

    _search_index_map["hwsingle_diff2_ge"] = bind(&hsingle_search_index::do_hsingle_diff2_ge, _hwsingle_index, _1, _2, _3);
    _search_index_map["hwsingle_diff2_le"] = bind(&hsingle_search_index::do_hsingle_diff2_le, _hwsingle_index, _1, _2, _3);

    _search_index_map["hwsingle_diff_ge_num_ge"] = bind(&hsingle_search_index::do_hsingle_diff_ge_num_ge, _hwsingle_index, _1, _2, _3);

    _search_index_map["hwsingle_diff_sum_ge"] = bind(&hsingle_search_index::do_hsingle_sum_ge, _hwsingle_index, _1, _2, _3);

    _search_index_map["hwsingle_diff_sum_le"] = bind(&hsingle_search_index::do_hsingle_sum_le, _hwsingle_index, _1, _2, _3);
#endif
}

base_search_index proc_data::get_search_index(string & key)
{
    auto ii = _search_index_map.find(key);
    if (ii == _search_index_map.end())
    {
        return NULL;
    }

    return ii->second;
}

void proc_data::reg_handler()
{
    shared_ptr<url_handler> tmp_hander = make_shared<uhandler_select>();
    _uhandler_map.insert(make_pair("/select", tmp_hander));

    tmp_hander = make_shared<uhandler_queryid>();
    _uhandler_map.insert(make_pair("/queryid", tmp_hander));

    tmp_hander = make_shared<uhandler_default>();
    _uhandler_map.insert(make_pair("/default", tmp_hander));
}

shared_ptr<url_handler> proc_data::get_url_handler(string & key)
{
    auto ii = _uhandler_map.find(key);
    if (ii == _uhandler_map.end())
    {
        ii = _uhandler_map.find("/default");
        if (ii == _uhandler_map.end())
        {
            return NULL;
        }

        return ii->second;
    }

    return ii->second;

}

int proc_data::get_highest_index(const string & id, int date_index, int date_index_end, set<int> & res)
{
    proc_data* p_data = proc_data::instance();
    res.clear();

    hquotation_search_item * search_index = p_data->_hquotation_index->current();

    auto ii = search_index->id_quotation.find(id);
    if (ii == search_index->id_quotation.end())
    {
        return -1;
    }

    auto mm = search_index->id_crest.find(id);
    if (mm == search_index->id_crest.end())
    {
        return -1;
    }

    const deque< shared_ptr<quotation_t>> &  tt = ii->second;
    int len = tt.size();
    int index;
    if (len  < 1 + abs(date_index) || len < 1 + abs(date_index_end))
        return -1;

    set<int>::iterator it_begin, it_end, itr;

    it_begin = mm->second.lower_bound(len - abs(date_index) - 1);
    it_end = mm->second.upper_bound(len  -abs(date_index_end)- 1);

    float max = 0;
    for (itr = it_begin; itr != it_end; itr++)
    {   
        if (res.empty() || max < tt[*itr]->high)
        {   
            max = tt[*itr]->high;
            res.clear();

            res.insert(len - *itr - 1); 
        }   
        else if (max == tt[*itr]->high)
        {   
            res.insert(len - *itr - 1); 
        }   
    }  


    return 0;
}


int proc_data::get_crest_index(const string & id, int date_index, int date_index_end, set<int> & res)
{
    proc_data* p_data = proc_data::instance();
    res.clear();

    hquotation_search_item * search_index = p_data->_hquotation_index->current();

    auto ii = search_index->id_quotation.find(id);
    if (ii == search_index->id_quotation.end())
    {
        return -1;
    }

    auto mm = search_index->id_crest.find(id);
    if (mm == search_index->id_crest.end())
    {
        return -1;
    }

    const deque< shared_ptr<quotation_t>> &  tt = ii->second;
    int len = tt.size();
    int index;
    if (len  < 1 + abs(date_index) || len < 1 + abs(date_index_end))
        return -1;

    set<int>::iterator it_begin, it_end, itr;

    it_begin = mm->second.lower_bound(len - abs(date_index) - 1);
    it_end = mm->second.upper_bound(len  -abs(date_index_end)- 1);

    float max = 0;
    for (itr = it_begin; itr != it_end; itr++)
    {   
        res.insert(len - *itr - 1); 
    }  


    return 0;
}



int proc_data::get_lowest_index(const string & id, int date_index, int date_index_end, set<int> & res)
{
    proc_data* p_data = proc_data::instance();
    res.clear();

    hquotation_search_item * search_index = p_data->_hquotation_index->current();

    auto ii = search_index->id_quotation.find(id);
    if (ii == search_index->id_quotation.end())
    {
        return -1;
    }

    auto mm = search_index->id_trough.find(id);
    if (mm == search_index->id_trough.end())
    {
        return -1;
    }

    const deque< shared_ptr<quotation_t>> &  tt = ii->second;
    int len = tt.size();
    int index;
    if (len  < 1 + abs(date_index) || len < 1 + abs(date_index_end))
        return -1;

    set<int>::iterator it_begin, it_end, itr;

    it_begin = mm->second.lower_bound(len - abs(date_index) - 1);
    it_end = mm->second.upper_bound(len  -abs(date_index_end)- 1);

    float max = 0;
    for (itr = it_begin; itr != it_end; itr++)
    {
        if (res.empty() || max > tt[*itr]->low)
        {
            max = tt[*itr]->low;
            res.clear();

            res.insert(len - *itr - 1);
        }
        else if (max == tt[*itr]->low)
        {
            res.insert(len - *itr - 1);
        }
    }


    return 0;
}


int proc_data::get_trough_index(const string & id, int date_index, int date_index_end, set<int> & res)
{
    proc_data* p_data = proc_data::instance();
    res.clear();

    hquotation_search_item * search_index = p_data->_hquotation_index->current();

    auto ii = search_index->id_quotation.find(id);
    if (ii == search_index->id_quotation.end())
    {
        return -1;
    }

    auto mm = search_index->id_trough.find(id);
    if (mm == search_index->id_trough.end())
    {
        return -1;
    }

    const deque< shared_ptr<quotation_t>> &  tt = ii->second;
    int len = tt.size();
    int index;
    if (len  < 1 + abs(date_index) || len < 1 + abs(date_index_end))
        return -1;

    set<int>::iterator it_begin, it_end, itr;

    it_begin = mm->second.lower_bound(len - abs(date_index) - 1);
    it_end = mm->second.upper_bound(len  -abs(date_index_end)- 1);

    float max = 0;
    for (itr = it_begin; itr != it_end; itr++)
    {
        res.insert(len - *itr - 1);
    }


    return 0;
}



void proc_data::reg_search_sstr()
{
    _search_sstr_map["highest"] = bind(&proc_data::get_highest_index, _1, _2, _3, _4);
    _search_sstr_map["lowest"] = bind(&proc_data::get_lowest_index, _1, _2, _3, _4);
    _search_sstr_map["trough"] = bind(&proc_data::get_trough_index, _1, _2, _3, _4);
    _search_sstr_map["crest"] = bind(&proc_data::get_crest_index, _1, _2, _3, _4);
}

int proc_data::get_date_index(const string & id, string & date_index, string & date_index_end, search_res & search, set<int> & date_index_out, set<int> & date_index_end_out)
{
    date_index_out.clear();
    date_index_end_out.clear();

    vector<string> tmp_vec;
    SplitString(date_index.c_str(), ',', &tmp_vec, SPLIT_MODE_ONE);
    if (!tmp_vec.size())
    {
        tmp_vec.push_back(date_index);
    }

    if (search.exist_key_index(tmp_vec[0]))
    {
        search.get_index_bykey(tmp_vec[0], id, date_index_out);
    }
    else
    {
        date_index_out.insert(abs(atoi(tmp_vec[0].c_str())));
    }


    vector<string> t_vec;
    SplitString(date_index_end.c_str(), ',', &t_vec, SPLIT_MODE_ONE);
    if (!t_vec.size())
    {
        t_vec.push_back(date_index_end);
    }

    if (search.exist_key_index(t_vec[0]))
    {
        search.get_index_bykey(t_vec[0], id, date_index_end_out);
    }
    else
    {
        date_index_end_out.insert(abs(atoi(t_vec[0].c_str())));
    }

    {
        set<int> out;
        if (tmp_vec.size()>= 2 && !tmp_vec[1].empty())
        {

            for (auto date_index: date_index_out)
            {
                set<int> t;
                for (auto date_index_end: date_index_end_out)
                {
                    if (date_index < date_index_end)
                        continue;

                    get_search_sstr(id, tmp_vec[1], date_index, date_index_end, search, t); 
                }

                out.insert(t.begin(), t.end());
            }

            date_index_out = out;
        }
    }

    {
        set<int> out;
        if (t_vec.size()>= 2 && !t_vec[1].empty())
        {

            for (auto date_index: date_index_out)
            {
                set<int> t;
                for (auto date_index_end: date_index_end_out)
                {
                    if (date_index < date_index_end)
                        continue;

                    get_search_sstr(id, t_vec[1], date_index, date_index_end, search, t); 
                }

                out.insert(t.begin(), t.end());
            }

            date_index_end_out = out;
        }

    }

    return 0;
}


int proc_data::get_search_sstr(const string & id, const string & sstr, int date_index, int date_index_end, search_res & search, set<int> & res)
{
    res.clear();
    vector<string> vec;
    parse_sstr(sstr, vec);
    if (vec.empty())
        return -1;

    date_index =  abs(date_index);
    date_index_end = abs(date_index_end);
    set<int> index_set;
    int ret = 0;
    int tmp;
    
    if (search.exist_key_index(vec[0]))
    {
        search.get_index_bykey(vec[0], id, index_set);
    }

//    ret = _lrussr_index->get_search_sstr(id, vec[0], date_index, date_index_end);
  //  if (ret < 0)
    //{
    else 
    {
        auto ii = _search_sstr_map.find(vec[0]);
        if (ii == _search_sstr_map.end())
        {
            return -1;
        }

        ret = ii->second(id, date_index, date_index_end, index_set);
    }
    //}

    for (auto ii = index_set.begin(); ii != index_set.end(); ii++)
    {
        tmp = *ii;
        ret =  *ii;
            //_lrussr_index->add_search_sstr(id, vec[0], date_index, date_index_end, ret);

        if (vec.size() == 3)
        {
            if (vec[1]  == "+")
            {
                tmp = ret + atoi(vec[2].c_str());           
            }
            else if (vec[2] == "-")
            {
                tmp = ret - atoi(vec[2].c_str());
            }

            ret = tmp;
        }

       res.insert(ret);
    }

    return 0;
}

void proc_data::parse_sstr(const string & value, vector<string> & vec)
{
    vec.clear();
    int pos = 0 ;
    for (int i = 0 ; i < (int)value.size(); i++)
    {
        if (value[i] == '+' || value[i] == '-' )
        {
            string str;
            if (i - pos > 0)
           {
               str = value.substr(pos, i - pos);
                str = StringTrim(str);
                if (!str.empty())
                    vec.push_back(str);
            }
            vec.push_back(string(1, value[i]));
            pos = i + 1;
        }
    }

    if (pos != (int)value.size())
    {   
        string str = value.substr(pos, (int)value.size() - pos);
        str = StringTrim(str);
        if (!str.empty())
            vec.push_back(str);
    }   

}


void proc_data::update_req_date(const string & date)
{
    _req_date.idle()->assign(date);
    _req_date.idle_2_current();
}

void proc_data::update_trade_date(const string & date)
{
    _trade_date.idle()->assign(date);
    _trade_date.idle_2_current();
}

const string * proc_data::get_req_date()
{
    return _req_date.current();
}

const string * proc_data::get_trade_date()
{
    return _trade_date.current();
}
