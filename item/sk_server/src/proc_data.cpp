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
#include "uhandler_select.h"
#include "uhandler_queryid.h"
#include "uhandler_default.h"

int proc_data::init(sk_conf * conf)
{
    get_proc_name(proc_name, sizeof(proc_name));

    reg_search_index();
    reg_handler();

    _conf = conf;
    
    {
        ua_dict *ua_dict1 = new (std::nothrow)ua_dict();
        ASSERT_WARNING(ua_dict1 != NULL, "allocate ua_dict fail");
        ua_dict1->init(_conf->_strategy->current()->ua_path.c_str(), 
                _conf->_strategy->current()->ua_file.c_str(), _conf->dump_dir.c_str());

        ua_dict *ua_dict2 = new (std::nothrow)ua_dict();
        ASSERT_WARNING(ua_dict2 != NULL, "allocate ua_dict fail");
        ua_dict2->init(_conf->_strategy->current()->ua_path.c_str(),
                _conf->_strategy->current()->ua_file.c_str(), _conf->dump_dir.c_str());

        _ua_dict = new (std::nothrow)reload_mgr<ua_dict>(ua_dict1, ua_dict2);
    }

    {
        id_dict *id_dict1 = new (std::nothrow)id_dict();
        ASSERT_WARNING(id_dict1 != NULL, "allocate id_dict fail");
        id_dict1->init(_conf->_strategy->current()->id_path.c_str(), 
                _conf->_strategy->current()->id_file.c_str(), _conf->dump_dir.c_str());

        id_dict *id_dict2 = new (std::nothrow)id_dict();
        ASSERT_WARNING(id_dict2 != NULL, "allocate id_dict fail");
        id_dict2->init(_conf->_strategy->current()->id_path.c_str(), 
                _conf->_strategy->current()->id_file.c_str(), _conf->dump_dir.c_str());

        _id_dict = new (std::nothrow)reload_mgr<id_dict>(id_dict1, id_dict2);
    }

    {
        addr_dict_split *addr_dict1 = new (std::nothrow)addr_dict_split();
        ASSERT_WARNING(addr_dict1 != NULL, "allocate addr_dict_split fail");
        addr_dict1->init(_conf->_strategy->current()->addr_dict_split_path.c_str(),
                _conf->_strategy->current()->addr_dict_split_file.c_str(), _conf->dump_dir.c_str());

        addr_dict_split *addr_dict2 = new (std::nothrow)addr_dict_split();
        ASSERT_WARNING(addr_dict2 != NULL, "allocate addr_dict_split fail");
        addr_dict2->init(_conf->_strategy->current()->addr_dict_split_path.c_str(),
                _conf->_strategy->current()->addr_dict_split_file.c_str(), _conf->dump_dir.c_str());

        _addr_dict_split = new (std::nothrow)reload_mgr<addr_dict_split>(addr_dict1, addr_dict2);
    }

    {
        addr_dict *addr_dict1 = new (std::nothrow)addr_dict();
        ASSERT_WARNING(addr_dict1 != NULL, "allocate addr_dict fail");
        addr_dict1->init(_conf->_strategy->current()->addr_dict_path.c_str(),
                _conf->_strategy->current()->addr_dict_file.c_str(), _conf->dump_dir.c_str());

        addr_dict *addr_dict2 = new (std::nothrow)addr_dict();
        ASSERT_WARNING(addr_dict2 != NULL, "allocate addr_dict fail");
        addr_dict2->init(_conf->_strategy->current()->addr_dict_path.c_str(),
                _conf->_strategy->current()->addr_dict_file.c_str(), _conf->dump_dir.c_str());

        _addr_dict = new (std::nothrow)reload_mgr<addr_dict>(addr_dict1, addr_dict2);
    }

    {
        plate_dict_split *addr_dict1 = new (std::nothrow)plate_dict_split();
        ASSERT_WARNING(addr_dict1 != NULL, "allocate plate_dict_split fail");
        addr_dict1->init(_conf->_strategy->current()->plate_dict_split_path.c_str(),
                _conf->_strategy->current()->plate_dict_split_file.c_str(), _conf->dump_dir.c_str());

        plate_dict_split *addr_dict2 = new (std::nothrow)plate_dict_split();
        ASSERT_WARNING(addr_dict2 != NULL, "allocate plate_dict_split fail");
        addr_dict2->init(_conf->_strategy->current()->plate_dict_split_path.c_str(),
                _conf->_strategy->current()->plate_dict_split_file.c_str(), _conf->dump_dir.c_str());

        _plate_dict_split = new (std::nothrow)reload_mgr<plate_dict_split>(addr_dict1, addr_dict2);
    }

    {
        plate_dict *addr_dict1 = new (std::nothrow)plate_dict();
        ASSERT_WARNING(addr_dict1 != NULL, "allocate plate_dict fail");
        addr_dict1->init(_conf->_strategy->current()->plate_dict_path.c_str(),
                _conf->_strategy->current()->plate_dict_file.c_str(), _conf->dump_dir.c_str());

        plate_dict *addr_dict2 = new (std::nothrow)plate_dict();
        ASSERT_WARNING(addr_dict2 != NULL, "allocate plate_dict fail");
        addr_dict2->init(_conf->_strategy->current()->plate_dict_path.c_str(),
                _conf->_strategy->current()->plate_dict_file.c_str(), _conf->dump_dir.c_str());

        _plate_dict = new (std::nothrow)reload_mgr<plate_dict>(addr_dict1, addr_dict2);
    }

    //id_rdict *id_dict1 = new (std::nothrow)id_rdict();
    //ASSERT_WARNING(id_dict1 != NULL, "allocate id_dict fail");
    //id_dict1->init(_conf->id_path.c_str(), _conf->id_file.c_str(), _conf->id_num, _conf->dump_dir.c_str());
    
    //id_rdict *id_dict2 = new (std::nothrow)id_rdict();
    //ASSERT_WARNING(id_dict2 != NULL, "allocate id_dict fail");
    //id_dict2->init(_conf->id_path.c_str(), _conf->id_file.c_str(), _conf->id_num, _conf->dump_dir.c_str());

    //_id_dict = new (std::nothrow)reload_mgr<id_rdict>(id_dict1, id_dict2);


    
    {
        _finance_dict.init(_conf->_strategy->current()->financie_path.c_str(),  
                conf->_strategy->current()->financie_file.c_str(), conf->dump_dir.c_str());
    }


    {
        ban_dict *b_dict1 = new (std::nothrow)ban_dict();
        ASSERT_WARNING(b_dict1 != NULL, "allocate ban_dict fail");
        b_dict1->init(_conf->_strategy->current()->ban_path.c_str(), 
                _conf->_strategy->current()->ban_file.c_str(), conf->dump_dir.c_str());

        ban_dict *b_dict2 = new (std::nothrow)ban_dict();
        ASSERT_WARNING(b_dict2 != NULL, "allocate ban_dict fail");
        b_dict2->init(_conf->_strategy->current()->ban_path.c_str(), 
                _conf->_strategy->current()->ban_file.c_str(), conf->dump_dir.c_str());

        _ban_dict = new (std::nothrow)reload_mgr<ban_dict>(b_dict1, b_dict2);
    }
    
    {
        history_single_dict *hs_dict1 = new (std::nothrow)history_single_dict();
        ASSERT_WARNING(hs_dict1 != NULL, "allocate history_single_t fail");
        hs_dict1->init(_conf->_strategy->current()->history_single_path.c_str(), 
                _conf->_strategy->current()->history_single_file.c_str(), conf->dump_dir.c_str());

        history_single_dict *hs_dict2 = new (std::nothrow)history_single_dict();
        ASSERT_WARNING(hs_dict2 != NULL, "allocate history_single_t fail");
        hs_dict2->init(_conf->_strategy->current()->history_single_path.c_str(), 
                _conf->_strategy->current()->history_single_file.c_str(), conf->dump_dir.c_str());

        _hsingle_dict = new (std::nothrow)reload_mgr<history_single_dict>(hs_dict1, hs_dict2);
    }


    {
        history_quotation_dict *hq_dict1 = new (std::nothrow)history_quotation_dict();
        ASSERT_WARNING(hq_dict1 != NULL, "allocate history_quotation_t fail");
        hq_dict1->init(_conf->_strategy->current()->history_quotation_path.c_str(),
                _conf->_strategy->current()->history_quotation_file.c_str(), conf->dump_dir.c_str());

        history_quotation_dict *hq_dict2 = new (std::nothrow)history_quotation_dict();
        ASSERT_WARNING(hq_dict2 != NULL, "allocate history_quotation_t fail");
        hq_dict2->init(_conf->_strategy->current()->history_quotation_path.c_str(),
                _conf->_strategy->current()->history_quotation_file.c_str(), conf->dump_dir.c_str());

        _hquoation_dict = new (std::nothrow)reload_mgr<history_quotation_dict>(hq_dict1, hq_dict2);
    }

    {
        holiday_dict *td_dict1 = new (std::nothrow)holiday_dict();
        ASSERT_WARNING(td_dict1 != NULL, "allocate holiday_dict fail");
        td_dict1->init(_conf->_strategy->current()->holiday_dict_path.c_str(), 
                _conf->_strategy->current()->holiday_dict_file.c_str(), conf->dump_dir.c_str());

        holiday_dict *td_dict2 = new (std::nothrow)holiday_dict();
        ASSERT_WARNING(td_dict2 != NULL, "allocate holiday_dict fail");
        td_dict2->init(_conf->_strategy->current()->holiday_dict_path.c_str(),
                _conf->_strategy->current()->holiday_dict_file.c_str(), conf->dump_dir.c_str());

        _holiday_dict = new (std::nothrow)reload_mgr<holiday_dict>(td_dict1, td_dict2);

    }

    return 0;
}

proc_data* proc_data::_singleton;

proc_data * proc_data::instance()
{
    if(!_singleton) 
    {
        _singleton = new(std::nothrow)proc_data();
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

    _hsingle_dict->load();

    _hquoation_dict->load();

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

    if (_hsingle_dict && _hsingle_dict->need_reload())
    {
        _hsingle_dict->reload();
        flag = 1;
    }

    if (_hquoation_dict && _hquoation_dict->need_reload())
    {
        _hquoation_dict->reload();
        flag = 1;
    }

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

    _hsingle_dict->dump();

    _hquoation_dict->dump();

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

    _hsingle_dict->destroy();

    _hquoation_dict->destroy();

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

    _hsingle_dict->idle()->destroy();

    _hquoation_dict->idle()->destroy();

    _holiday_dict->idle()->destroy();

    return 0;
}

void proc_data::reg_search_index()
{
    _block_set = std::make_shared<block_search_index>();
    _search_index_map.insert(std::make_pair("block", _block_set));
    _search_index_map.insert(std::make_pair("block_v", _block_set));

    _end_index = std::make_shared<end_search_index>();
    _search_index_map.insert(std::make_pair("end_ge", _end_index));
    _search_index_map.insert(std::make_pair("end_ge_v", _end_index));
    _search_index_map.insert(std::make_pair("end_le", _end_index));
    _search_index_map.insert(std::make_pair("end_le_v", _end_index));

    _change_rate_index = std::make_shared<change_rate_search_index>();
    _search_index_map.insert(std::make_pair("change_rate_ge", _change_rate_index));
    _search_index_map.insert(std::make_pair("change_rate_le", _change_rate_index));
    _search_index_map.insert(std::make_pair("change_rate_ge_v", _change_rate_index));
    _search_index_map.insert(std::make_pair("change_rate_le_v", _change_rate_index));


    _range_percent_index = std::make_shared<range_percent_search_index>();
    _search_index_map.insert(std::make_pair("range_percent_ge", _range_percent_index));
    _search_index_map.insert(std::make_pair("range_percent_ge_v", _range_percent_index));
    _search_index_map.insert(std::make_pair("range_percent_le", _range_percent_index));
    _search_index_map.insert(std::make_pair("range_percent_le_v", _range_percent_index));

    _down_pointer_index = std::make_shared<down_pointer_search_index>();
    _search_index_map.insert(std::make_pair("down_pointer_ge", _down_pointer_index));
    _search_index_map.insert(std::make_pair("down_pointer_le", _down_pointer_index));
    _search_index_map.insert(std::make_pair("down_pointer_ge_v", _down_pointer_index));
    _search_index_map.insert(std::make_pair("down_pointer_le_v", _down_pointer_index));

    _up_pointer_index = std::make_shared<up_pointer_search_index>();
    _search_index_map.insert(std::make_pair("up_pointer_ge", _up_pointer_index));
    _search_index_map.insert(std::make_pair("up_pointer_le", _up_pointer_index));
    _search_index_map.insert(std::make_pair("up_pointer_ge_v", _up_pointer_index));
    _search_index_map.insert(std::make_pair("up_pointer_le_v", _up_pointer_index));

    _end_avg_end_index = std::make_shared<end_avg_end_search_index>();
    _search_index_map.insert(std::make_pair("end_avg_end_ge", _end_avg_end_index));
    _search_index_map.insert(std::make_pair("end_avg_end_le", _end_avg_end_index));
    _search_index_map.insert(std::make_pair("end_avg_end_ge_v", _end_avg_end_index));
    _search_index_map.insert(std::make_pair("end_avg_end_le_v", _end_avg_end_index));

    _end_avg_end5_index = std::make_shared<end_avg_end5_search_index>();
    _search_index_map.insert(std::make_pair("end_avg_end5_ge", _end_avg_end5_index));
    _search_index_map.insert(std::make_pair("end_avg_end5_le", _end_avg_end5_index));
    _search_index_map.insert(std::make_pair("end_avg_end5_ge_v", _end_avg_end5_index));
    _search_index_map.insert(std::make_pair("end_avg_end5_le_v", _end_avg_end5_index));

    _end_avg_end10_index = std::make_shared<end_avg_end10_search_index>();
    _search_index_map.insert(std::make_pair("end_avg_end10_ge", _end_avg_end10_index));
    _search_index_map.insert(std::make_pair("end_avg_end10_le", _end_avg_end10_index));
    _search_index_map.insert(std::make_pair("end_avg_end10_ge_v", _end_avg_end10_index));
    _search_index_map.insert(std::make_pair("end_avg_end10_le_v", _end_avg_end10_index));

    _end_avg_end20_index = std::make_shared<end_avg_end20_search_index>();
    _search_index_map.insert(std::make_pair("end_avg_end20_ge", _end_avg_end20_index));
    _search_index_map.insert(std::make_pair("end_avg_end20_le", _end_avg_end20_index));
    _search_index_map.insert(std::make_pair("end_avg_end20_ge_v", _end_avg_end20_index));
    _search_index_map.insert(std::make_pair("end_avg_end20_le_v", _end_avg_end20_index));

    _end_avg_end30_index = std::make_shared<end_avg_end30_search_index>();
    _search_index_map.insert(std::make_pair("end_avg_end30_ge", _end_avg_end30_index));
    _search_index_map.insert(std::make_pair("end_avg_end30_le", _end_avg_end30_index));
    _search_index_map.insert(std::make_pair("end_avg_end30_ge_v", _end_avg_end30_index));
    _search_index_map.insert(std::make_pair("end_avg_end30_le_v", _end_avg_end30_index));

    _end_end5_index = std::make_shared<end_end5_search_index>();
    _search_index_map.insert(std::make_pair("end_end5_ge", _end_end5_index));
    _search_index_map.insert(std::make_pair("end_end5_le", _end_end5_index));
    _search_index_map.insert(std::make_pair("end_end5_ge_v", _end_end5_index));
    _search_index_map.insert(std::make_pair("end_end5_le_v", _end_end5_index));

    _end_end10_index = std::make_shared<end_end10_search_index>();
    _search_index_map.insert(std::make_pair("end_end10_ge", _end_end10_index));
    _search_index_map.insert(std::make_pair("end_end10_le", _end_end10_index));
    _search_index_map.insert(std::make_pair("end_end10_ge_v", _end_end10_index));
    _search_index_map.insert(std::make_pair("end_end10_le_v", _end_end10_index));


    _end_end20_index = std::make_shared<end_end20_search_index>();
    _search_index_map.insert(std::make_pair("end_end20_ge", _end_end20_index));
    _search_index_map.insert(std::make_pair("end_end20_le", _end_end20_index));
    _search_index_map.insert(std::make_pair("end_end20_ge_v", _end_end20_index));
    _search_index_map.insert(std::make_pair("end_end20_le_v", _end_end20_index));

    _end_end30_index = std::make_shared<end_end30_search_index>();
    _search_index_map.insert(std::make_pair("end_end30_ge", _end_end30_index));
    _search_index_map.insert(std::make_pair("end_end30_le", _end_end30_index));
    _search_index_map.insert(std::make_pair("end_end30_ge_v", _end_end30_index));
    _search_index_map.insert(std::make_pair("end_end30_le_v", _end_end30_index));

    _pe_index = std::make_shared<pe_search_index>();
    _search_index_map.insert(std::make_pair("pe_ge", _pe_index));
    _search_index_map.insert(std::make_pair("pe_le", _pe_index));
    _search_index_map.insert(std::make_pair("pe_ge_v", _pe_index));
    _search_index_map.insert(std::make_pair("pe_le_v", _pe_index));

    _pb_index = std::make_shared<pb_search_index>();
    _search_index_map.insert(std::make_pair("pb_ge", _pb_index));
    _search_index_map.insert(std::make_pair("pb_le", _pb_index));
    _search_index_map.insert(std::make_pair("pb_ge_v", _pb_index));
    _search_index_map.insert(std::make_pair("pb_le_v", _pb_index));

    _value_index = std::make_shared<value_search_index>();
    _search_index_map.insert(std::make_pair("value_ge", _value_index));
    _search_index_map.insert(std::make_pair("value_le", _value_index));
    _search_index_map.insert(std::make_pair("value_ge_v", _value_index));
    _search_index_map.insert(std::make_pair("value_le_v", _value_index));

    _cir_value_index = std::make_shared<cir_value_search_index>();
    _search_index_map.insert(std::make_pair("cir_value_ge", _cir_value_index));
    _search_index_map.insert(std::make_pair("cir_value_le", _cir_value_index));
    _search_index_map.insert(std::make_pair("cir_value_ge_v", _cir_value_index));
    _search_index_map.insert(std::make_pair("cir_value_le_v", _cir_value_index));

    _mgxj_index = std::make_shared<mgxj_search_index>();
    _search_index_map.insert(std::make_pair("mgxj_ge", _mgxj_index));
    _search_index_map.insert(std::make_pair("mgxj_le", _mgxj_index));
    _search_index_map.insert(std::make_pair("mgxj_ge_v", _mgxj_index));
    _search_index_map.insert(std::make_pair("mgxj_le_v", _mgxj_index));

    _mgsy_index = std::make_shared<mgsy_search_index>();
    _search_index_map.insert(std::make_pair("mgsy_ge", _mgsy_index));
    _search_index_map.insert(std::make_pair("mgsy_le", _mgsy_index));
    _search_index_map.insert(std::make_pair("mgsy_ge_v", _mgsy_index));
    _search_index_map.insert(std::make_pair("mgsy_le_v", _mgsy_index));

    _mgsygr_index = std::make_shared<mgsygr_search_index>();
    _search_index_map.insert(std::make_pair("mgsygr_ge", _mgsygr_index));
    _search_index_map.insert(std::make_pair("mgsygr_le", _mgsygr_index));
    _search_index_map.insert(std::make_pair("mgsygr_ge_v", _mgsygr_index));
    _search_index_map.insert(std::make_pair("mgsygr_le_v", _mgsygr_index));

    _mgxjgr_index = std::make_shared<mgxjgr_search_index>();
    _search_index_map.insert(std::make_pair("mgxjgr_ge", _mgxjgr_index));
    _search_index_map.insert(std::make_pair("mgxjgr_le", _mgxjgr_index));
    _search_index_map.insert(std::make_pair("mgxjgr_ge_v", _mgxjgr_index));
    _search_index_map.insert(std::make_pair("mgxjgr_le_v", _mgxjgr_index));

    _zysrgr_index = std::make_shared<zysrgr_search_index>();
    _search_index_map.insert(std::make_pair("zysrgr_ge", _zysrgr_index));
    _search_index_map.insert(std::make_pair("zysrgr_le", _zysrgr_index));
    _search_index_map.insert(std::make_pair("zysrgr_ge_v", _zysrgr_index));
    _search_index_map.insert(std::make_pair("zysrgr_le_v", _zysrgr_index));

    _yylrgr_index = std::make_shared<yylrgr_search_index>();
    _search_index_map.insert(std::make_pair("yylrgr_ge", _yylrgr_index));
    _search_index_map.insert(std::make_pair("yylrgr_le", _yylrgr_index));
    _search_index_map.insert(std::make_pair("yylrgr_ge_v", _yylrgr_index));
    _search_index_map.insert(std::make_pair("yylrgr_le_v", _yylrgr_index));

    _jlrgr_index = std::make_shared<jlrgr_search_index>();
    _search_index_map.insert(std::make_pair("jlrgr_ge", _jlrgr_index));
    _search_index_map.insert(std::make_pair("jlrgr_le", _jlrgr_index));
    _search_index_map.insert(std::make_pair("jlrgr_ge_v", _jlrgr_index));
    _search_index_map.insert(std::make_pair("jlrgr_le_v", _jlrgr_index));

    _address_index = std::make_shared<address_search_index>();
    _search_index_map.insert(std::make_pair("address", _address_index));
    _search_index_map.insert(std::make_pair("address_v", _address_index));

    _plate_index = std::make_shared<plate_search_index>();
    _search_index_map.insert(std::make_pair("plate", _plate_index));
    _search_index_map.insert(std::make_pair("plate_v", _plate_index));

    _rsingle_diff_index = std::make_shared<rsingle_diff_search_index>();
    _search_index_map.insert(std::make_pair("rsingle_diff_0_ge", _rsingle_diff_index));
    _search_index_map.insert(std::make_pair("rsingle_diff_1_ge", _rsingle_diff_index));
    _search_index_map.insert(std::make_pair("rsingle_diff_2_ge", _rsingle_diff_index));
    _search_index_map.insert(std::make_pair("rsingle_diff_0_ge_v", _rsingle_diff_index));
    _search_index_map.insert(std::make_pair("rsingle_diff_1_ge_v", _rsingle_diff_index));
    _search_index_map.insert(std::make_pair("rsingle_diff_2_ge_v", _rsingle_diff_index));
    _search_index_map.insert(std::make_pair("rsingle_diff_0_le", _rsingle_diff_index));
    _search_index_map.insert(std::make_pair("rsingle_diff_1_le", _rsingle_diff_index));
    _search_index_map.insert(std::make_pair("rsingle_diff_2_le", _rsingle_diff_index));
    _search_index_map.insert(std::make_pair("rsingle_diff_0_le_v", _rsingle_diff_index));
    _search_index_map.insert(std::make_pair("rsingle_diff_1_le_v", _rsingle_diff_index));
    _search_index_map.insert(std::make_pair("rsingle_diff_2_le_v", _rsingle_diff_index));

    _rsingle_diff2_index = std::make_shared<rsingle_diff2_search_index>();
    _search_index_map.insert(std::make_pair("rsingle_diff2_0_ge", _rsingle_diff2_index));
    _search_index_map.insert(std::make_pair("rsingle_diff2_1_ge", _rsingle_diff2_index));
    _search_index_map.insert(std::make_pair("rsingle_diff2_2_ge", _rsingle_diff2_index));
    _search_index_map.insert(std::make_pair("rsingle_diff2_0_le", _rsingle_diff2_index));
    _search_index_map.insert(std::make_pair("rsingle_diff2_1_le", _rsingle_diff2_index));
    _search_index_map.insert(std::make_pair("rsingle_diff2_2_le", _rsingle_diff2_index));
    _search_index_map.insert(std::make_pair("rsingle_diff2_0_ge_v", _rsingle_diff2_index));
    _search_index_map.insert(std::make_pair("rsingle_diff2_1_ge_v", _rsingle_diff2_index));
    _search_index_map.insert(std::make_pair("rsingle_diff2_2_ge_v", _rsingle_diff2_index));
    _search_index_map.insert(std::make_pair("rsingle_diff2_0_le_v", _rsingle_diff2_index));
    _search_index_map.insert(std::make_pair("rsingle_diff2_1_le_v", _rsingle_diff2_index));
    _search_index_map.insert(std::make_pair("rsingle_diff2_2_le_v", _rsingle_diff2_index));

    _hsingle_diff_index = std::make_shared<hsingle_diff_search_index>();
    _search_index_map.insert(std::make_pair("hsingle_diff_0_ge", _hsingle_diff_index));
    _search_index_map.insert(std::make_pair("hsingle_diff_1_ge", _hsingle_diff_index));
    _search_index_map.insert(std::make_pair("hsingle_diff_2_ge", _hsingle_diff_index));
    _search_index_map.insert(std::make_pair("hsingle_diff_0_le", _hsingle_diff_index));
    _search_index_map.insert(std::make_pair("hsingle_diff_1_le", _hsingle_diff_index));
    _search_index_map.insert(std::make_pair("hsingle_diff_2_le", _hsingle_diff_index));
    _search_index_map.insert(std::make_pair("hsingle_diff_0_ge_v", _hsingle_diff_index));
    _search_index_map.insert(std::make_pair("hsingle_diff_1_ge_v", _hsingle_diff_index));
    _search_index_map.insert(std::make_pair("hsingle_diff_2_ge_v", _hsingle_diff_index));
    _search_index_map.insert(std::make_pair("hsingle_diff_0_le_v", _hsingle_diff_index));
    _search_index_map.insert(std::make_pair("hsingle_diff_1_le_v", _hsingle_diff_index));
    _search_index_map.insert(std::make_pair("hsingle_diff_2_le_v", _hsingle_diff_index));

    _hsingle_sum_diff_index = std::make_shared<hsingle_sum_diff_search_index>();
    _search_index_map.insert(std::make_pair("hsingle_sum_diff_0_ge", _hsingle_sum_diff_index));
    _search_index_map.insert(std::make_pair("hsingle_sum_diff_1_ge", _hsingle_sum_diff_index));
    _search_index_map.insert(std::make_pair("hsingle_sum_diff_2_ge", _hsingle_sum_diff_index));
    _search_index_map.insert(std::make_pair("hsingle_sum_diff_0_le", _hsingle_sum_diff_index));
    _search_index_map.insert(std::make_pair("hsingle_sum_diff_1_le", _hsingle_sum_diff_index));
    _search_index_map.insert(std::make_pair("hsingle_sum_diff_2_le", _hsingle_sum_diff_index));
    _search_index_map.insert(std::make_pair("hsingle_sum_diff_0_ge_v", _hsingle_sum_diff_index));
    _search_index_map.insert(std::make_pair("hsingle_sum_diff_1_ge_v", _hsingle_sum_diff_index));
    _search_index_map.insert(std::make_pair("hsingle_sum_diff_2_ge_v", _hsingle_sum_diff_index));
    _search_index_map.insert(std::make_pair("hsingle_sum_diff_0_le_v", _hsingle_sum_diff_index));
    _search_index_map.insert(std::make_pair("hsingle_sum_diff_1_le_v", _hsingle_sum_diff_index));
    _search_index_map.insert(std::make_pair("hsingle_sum_diff_2_le_v", _hsingle_sum_diff_index));


    _hqchange_rate_index = std::make_shared<hqchange_rate_search_index>();
    _search_index_map.insert(std::make_pair("hqchange_rate_ge", _hqchange_rate_index));
    _search_index_map.insert(std::make_pair("hqchange_rate_le", _hqchange_rate_index));
    _search_index_map.insert(std::make_pair("hqchange_rate_ge_v", _hqchange_rate_index));
    _search_index_map.insert(std::make_pair("hqchange_rate_le_v", _hqchange_rate_index));

    _hqrange_percent_index = std::make_shared<hqrange_percent_search_index>();
    _search_index_map.insert(std::make_pair("hqrange_percent_ge", _hqrange_percent_index));
    _search_index_map.insert(std::make_pair("hqrange_percent_le", _hqrange_percent_index));
    _search_index_map.insert(std::make_pair("hqrange_percent_ge_v", _hqrange_percent_index));
    _search_index_map.insert(std::make_pair("hqrange_percent_le_v", _hqrange_percent_index));
    _search_index_map.insert(std::make_pair("hqrange_percent_ge_num_ge", _hqrange_percent_index));
    _search_index_map.insert(std::make_pair("hqrange_percent_ge_num_ge_v", _hqrange_percent_index));

    _hqdown_pointer_index = std::make_shared<hqdown_pointer_search_index>();
    _search_index_map.insert(std::make_pair("hqdown_pointer_ge", _hqdown_pointer_index));
    _search_index_map.insert(std::make_pair("hqdown_pointer_le", _hqdown_pointer_index));
    _search_index_map.insert(std::make_pair("hqdown_pointer_ge_v", _hqdown_pointer_index));
    _search_index_map.insert(std::make_pair("hqdown_pointer_le_v", _hqdown_pointer_index));
    _search_index_map.insert(std::make_pair("hqdown_pointer_ge_num_ge", _hqdown_pointer_index));
    _search_index_map.insert(std::make_pair("hqdown_pointer_ge_num_ge_v", _hqdown_pointer_index));

    _hqup_pointer_index = std::make_shared<hqup_pointer_search_index>();
    _search_index_map.insert(std::make_pair("hqup_pointer_ge", _hqup_pointer_index));
    _search_index_map.insert(std::make_pair("hqup_pointer_le", _hqup_pointer_index));
    _search_index_map.insert(std::make_pair("hqup_pointer_ge_v", _hqup_pointer_index));
    _search_index_map.insert(std::make_pair("hqup_pointer_le_v", _hqup_pointer_index));
    _search_index_map.insert(std::make_pair("hqup_pointer_ge_num_ge", _hqup_pointer_index));
    _search_index_map.insert(std::make_pair("hqup_pointer_ge_num_ge_v", _hqup_pointer_index));

    _hq_sum_range_percent_index = std::make_shared<hq_sum_range_percent_search_index>();
    _search_index_map.insert(std::make_pair("hq_sum_range_percent_ge", _hq_sum_range_percent_index));
    _search_index_map.insert(std::make_pair("hq_sum_range_percent_ge_v", _hq_sum_range_percent_index));
    _search_index_map.insert(std::make_pair("hq_sum_range_percent_le", _hq_sum_range_percent_index));
    _search_index_map.insert(std::make_pair("hq_sum_range_percent_le_v", _hq_sum_range_percent_index));

    _hq_sum_change_rate_index = std::make_shared<hq_sum_change_rate_search_index>();
    _search_index_map.insert(std::make_pair("hq_sum_change_rate_ge", _hq_sum_change_rate_index));
    _search_index_map.insert(std::make_pair("hq_sum_change_rate_ge_v", _hq_sum_change_rate_index));
    _search_index_map.insert(std::make_pair("hq_sum_change_rate_le", _hq_sum_change_rate_index));
    _search_index_map.insert(std::make_pair("hq_sum_change_rate_le_v", _hq_sum_change_rate_index));

    _hqend_hqstart_index = std::make_shared<hqend_hqstart_search_index>();
    _search_index_map.insert(std::make_pair("hqend_hqstart_ge", _hqend_hqstart_index));
    _search_index_map.insert(std::make_pair("hqend_hqstart_ge_v", _hqend_hqstart_index));
    _search_index_map.insert(std::make_pair("hqend_hqstart_le", _hqend_hqstart_index));
    _search_index_map.insert(std::make_pair("hqend_hqstart_le_v", _hqend_hqstart_index));
    _search_index_map.insert(std::make_pair("hqstart_ge_num_ge", _hqend_hqstart_index));
    _search_index_map.insert(std::make_pair("hqstart_ge_num_ge_v", _hqend_hqstart_index));
}

std::shared_ptr<base_search_index>  proc_data::get_search_index(std::string & key)
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
    std::shared_ptr<url_handler> tmp_hander = std::make_shared<uhandler_select>();
    _uhandler_map.insert(std::make_pair("/select", tmp_hander));

    tmp_hander = std::make_shared<uhandler_queryid>();
    _uhandler_map.insert(std::make_pair("/queryid", tmp_hander));

    tmp_hander = std::make_shared<uhandler_default>();
    _uhandler_map.insert(std::make_pair("/default", tmp_hander));
}

std::shared_ptr<url_handler> proc_data::get_url_handler(std::string & key)
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
