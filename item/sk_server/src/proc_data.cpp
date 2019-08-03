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
        _hsingle_dict = std::make_shared<history_single_dict>();
        _hsingle_dict->init(_conf->_strategy->current()->history_single_path.c_str(), 
                _conf->_strategy->current()->history_single_file.c_str(), conf->dump_dir.c_str());
    }


    {
        _hquoation_dict = std::make_shared<history_quotation_dict>();
        _hquoation_dict->init(_conf->_strategy->current()->history_quotation_path.c_str(),
                _conf->_strategy->current()->history_quotation_file.c_str(), conf->dump_dir.c_str());
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

    _holiday_dict->idle()->destroy();

    return 0;
}

void proc_data::reg_search_index()
{
    _block_set = std::make_shared<block_search_index>();
    _search_index_map["block"] = std::bind(&block_search_index::search, _block_set, _1, _2, _3);
    _search_index_map["block_v"] = std::bind(&block_search_index::search, _block_set, _1, _2, _3);

    _rquotation_index = std::make_shared<rquotation_search_index>();
    _search_index_map["end_le"] = std::bind(&rquotation_search_index::do_check_end_le, _rquotation_index, _1, _2, _3);
    _search_index_map["end_le_v"] = std::bind(&rquotation_search_index::do_check_end_le, _rquotation_index, _1, _2, _3);
    _search_index_map["end_ge"] = std::bind(&rquotation_search_index::do_check_end_ge, _rquotation_index, _1, _2, _3);
    _search_index_map["end_ge_v"] = std::bind(&rquotation_search_index::do_check_end_ge, _rquotation_index, _1, _2, _3);

    _search_index_map["change_rate_le"] = std::bind(&rquotation_search_index::do_check_change_rate_le, _rquotation_index, _1, _2, _3);
    _search_index_map["change_rate_le_v"] = std::bind(&rquotation_search_index::do_check_change_rate_le, _rquotation_index, _1, _2, _3);
    _search_index_map["change_rate_ge"] = std::bind(&rquotation_search_index::do_check_change_rate_ge, _rquotation_index, _1, _2, _3);
    _search_index_map["change_rate_ge_v"] = std::bind(&rquotation_search_index::do_check_change_rate_ge, _rquotation_index, _1, _2, _3);

    _search_index_map["range_percent_le"] = std::bind(&rquotation_search_index::do_check_range_percent_le, _rquotation_index, _1, _2, _3);
    _search_index_map["range_percent_le_v"] = std::bind(&rquotation_search_index::do_check_range_percent_le, _rquotation_index, _1, _2, _3);
    _search_index_map["range_percent_ge"] = std::bind(&rquotation_search_index::do_check_range_percent_ge, _rquotation_index, _1, _2, _3);
    _search_index_map["range_percent_ge_v"] = std::bind(&rquotation_search_index::do_check_range_percent_ge, _rquotation_index, _1, _2, _3);

    _search_index_map["down_pointer_le"] = std::bind(&rquotation_search_index::do_check_down_pointer_le, _rquotation_index, _1, _2, _3);
    _search_index_map["down_pointer_le_v"] = std::bind(&rquotation_search_index::do_check_down_pointer_le, _rquotation_index, _1, _2, _3);
    _search_index_map["down_pointer_ge"] = std::bind(&rquotation_search_index::do_check_down_pointer_ge, _rquotation_index, _1, _2, _3);
    _search_index_map["down_pointer_ge_v"] = std::bind(&rquotation_search_index::do_check_down_pointer_ge, _rquotation_index, _1, _2, _3);

    _search_index_map["up_pointer_le"] = std::bind(&rquotation_search_index::do_check_up_pointer_le, _rquotation_index, _1, _2, _3);
    _search_index_map["up_pointer_le_v"] = std::bind(&rquotation_search_index::do_check_up_pointer_le, _rquotation_index, _1, _2, _3);
    _search_index_map["up_pointer_ge"] = std::bind(&rquotation_search_index::do_check_up_pointer_ge, _rquotation_index, _1, _2, _3);
    _search_index_map["up_pointer_ge_v"] = std::bind(&rquotation_search_index::do_check_up_pointer_ge, _rquotation_index, _1, _2, _3);

    _search_index_map["end_avg_end_le"] = std::bind(&rquotation_search_index::do_check_end_avg_end_le, _rquotation_index, _1, _2, _3);
    _search_index_map["end_avg_end_le_v"] = std::bind(&rquotation_search_index::do_check_end_avg_end_le, _rquotation_index, _1, _2, _3);
    _search_index_map["end_avg_end_ge"] = std::bind(&rquotation_search_index::do_check_end_avg_end_ge, _rquotation_index, _1, _2, _3);
    _search_index_map["end_avg_end_ge_v"] = std::bind(&rquotation_search_index::do_check_end_avg_end_ge, _rquotation_index, _1, _2, _3);

    _search_index_map["end_end5_le"] = std::bind(&rquotation_search_index::do_check_end_end5_le, _rquotation_index, _1, _2, _3);
    _search_index_map["end_end5_le_v"] = std::bind(&rquotation_search_index::do_check_end_end5_le, _rquotation_index, _1, _2, _3);
    _search_index_map["end_end5_ge"] = std::bind(&rquotation_search_index::do_check_end_end5_ge, _rquotation_index, _1, _2, _3);
    _search_index_map["end_end5_ge_v"] = std::bind(&rquotation_search_index::do_check_end_end5_ge, _rquotation_index, _1, _2, _3);

    _search_index_map["end_end10_le"] = std::bind(&rquotation_search_index::do_check_end_end10_le, _rquotation_index, _1, _2, _3);
    _search_index_map["end_end10_le_v"] = std::bind(&rquotation_search_index::do_check_end_end10_le, _rquotation_index, _1, _2, _3);
    _search_index_map["end_end10_ge"] = std::bind(&rquotation_search_index::do_check_end_end10_ge, _rquotation_index, _1, _2, _3);
    _search_index_map["end_end10_ge_v"] = std::bind(&rquotation_search_index::do_check_end_end10_ge, _rquotation_index, _1, _2, _3);

    _search_index_map["end_end20_le"] = std::bind(&rquotation_search_index::do_check_end_end20_le, _rquotation_index, _1, _2, _3);
    _search_index_map["end_end20_le_v"] = std::bind(&rquotation_search_index::do_check_end_end20_le, _rquotation_index, _1, _2, _3);
    _search_index_map["end_end20_ge"] = std::bind(&rquotation_search_index::do_check_end_end20_ge, _rquotation_index, _1, _2, _3);
    _search_index_map["end_end20_ge_v"] = std::bind(&rquotation_search_index::do_check_end_end20_ge, _rquotation_index, _1, _2, _3);

    _search_index_map["end_end30_le"] = std::bind(&rquotation_search_index::do_check_end_end30_le, _rquotation_index, _1, _2, _3);
    _search_index_map["end_end30_le_v"] = std::bind(&rquotation_search_index::do_check_end_end30_le, _rquotation_index, _1, _2, _3);
    _search_index_map["end_end30_ge"] = std::bind(&rquotation_search_index::do_check_end_end30_ge, _rquotation_index, _1, _2, _3);
    _search_index_map["end_end30_ge_v"] = std::bind(&rquotation_search_index::do_check_end_end30_ge, _rquotation_index, _1, _2, _3);

    _finance_index = std::make_shared<finance_search_index>();
    _search_index_map["pe_le"] = std::bind(&finance_search_index::do_check_pe_le, _finance_index, _1, _2, _3);
    _search_index_map["pe_ge"] = std::bind(&finance_search_index::do_check_pe_ge, _finance_index, _1, _2, _3);
    _search_index_map["pe_le_v"] = std::bind(&finance_search_index::do_check_pe_le, _finance_index, _1, _2, _3);
    _search_index_map["pe_ge_v"] = std::bind(&finance_search_index::do_check_pe_ge, _finance_index, _1, _2, _3);

    _search_index_map["pb_le"] = std::bind(&finance_search_index::do_check_pb_le, _finance_index, _1, _2, _3);
    _search_index_map["pb_ge"] = std::bind(&finance_search_index::do_check_pb_ge, _finance_index, _1, _2, _3);
    _search_index_map["pb_le_v"] = std::bind(&finance_search_index::do_check_pb_le, _finance_index, _1, _2, _3);
    _search_index_map["pb_ge_v"] = std::bind(&finance_search_index::do_check_pb_ge, _finance_index, _1, _2, _3);

    _search_index_map["value_le"] = std::bind(&finance_search_index::do_check_value_le, _finance_index, _1, _2, _3);
    _search_index_map["value_ge"] = std::bind(&finance_search_index::do_check_value_ge, _finance_index, _1, _2, _3);
    _search_index_map["value_le_v"] = std::bind(&finance_search_index::do_check_value_le, _finance_index, _1, _2, _3);
    _search_index_map["value_ge_v"] = std::bind(&finance_search_index::do_check_value_ge, _finance_index, _1, _2, _3);

    _search_index_map["cir_value_le"] = std::bind(&finance_search_index::do_check_cir_value_le, _finance_index, _1, _2, _3);
    _search_index_map["cir_value_ge"] = std::bind(&finance_search_index::do_check_cir_value_ge, _finance_index, _1, _2, _3);
    _search_index_map["cir_value_le_v"] = std::bind(&finance_search_index::do_check_cir_value_le, _finance_index, _1, _2, _3);
    _search_index_map["cir_value_ge_v"] = std::bind(&finance_search_index::do_check_cir_value_ge, _finance_index, _1, _2, _3);

    _search_index_map["mgxj_le"] = std::bind(&finance_search_index::do_check_mgxj_le, _finance_index, _1, _2, _3);
    _search_index_map["mgxj_ge"] = std::bind(&finance_search_index::do_check_mgxj_ge, _finance_index, _1, _2, _3);
    _search_index_map["mgxj_le_v"] = std::bind(&finance_search_index::do_check_mgxj_le, _finance_index, _1, _2, _3);
    _search_index_map["mgxj_ge_v"] = std::bind(&finance_search_index::do_check_mgxj_ge, _finance_index, _1, _2, _3);

    _search_index_map["mgsy_le"] = std::bind(&finance_search_index::do_check_mgsy_le, _finance_index, _1, _2, _3);
    _search_index_map["mgsy_ge"] = std::bind(&finance_search_index::do_check_mgsy_ge, _finance_index, _1, _2, _3);
    _search_index_map["mgsy_le_v"] = std::bind(&finance_search_index::do_check_mgsy_le, _finance_index, _1, _2, _3);
    _search_index_map["mgsy_ge_v"] = std::bind(&finance_search_index::do_check_mgsy_ge, _finance_index, _1, _2, _3);

    _search_index_map["zysrgr_le"] = std::bind(&finance_search_index::do_check_zysrgr_le, _finance_index, _1, _2, _3);
    _search_index_map["zysrgr_ge"] = std::bind(&finance_search_index::do_check_zysrgr_ge, _finance_index, _1, _2, _3);
    _search_index_map["zysrgr_le_v"] = std::bind(&finance_search_index::do_check_zysrgr_le, _finance_index, _1, _2, _3);
    _search_index_map["zysrgr_ge_v"] = std::bind(&finance_search_index::do_check_zysrgr_ge, _finance_index, _1, _2, _3);

    _search_index_map["jlrgr_le"] = std::bind(&finance_search_index::do_check_jlrgr_le, _finance_index, _1, _2, _3);
    _search_index_map["jlrgr_ge"] = std::bind(&finance_search_index::do_check_jlrgr_ge, _finance_index, _1, _2, _3);
    _search_index_map["jlrgr_le_v"] = std::bind(&finance_search_index::do_check_jlrgr_le, _finance_index, _1, _2, _3);
    _search_index_map["jlrgr_ge_v"] = std::bind(&finance_search_index::do_check_jlrgr_ge, _finance_index, _1, _2, _3);

    _address_index = std::make_shared<address_search_index>();
    _search_index_map["address"] = std::bind(&address_search_index::search, _address_index, _1, _2, _3);
    _search_index_map["address_v"] = std::bind(&address_search_index::search, _address_index, _1, _2, _3);

    _plate_index = std::make_shared<plate_search_index>();
    _search_index_map["plate"] = std::bind(&plate_search_index::search, _plate_index, _1, _2, _3);
    _search_index_map["plate_v"] = std::bind(&plate_search_index::search, _plate_index, _1, _2, _3);

    _rsingle_index = std::make_shared<rsingle_search_index>();
    _search_index_map["rsingle_diff_0_ge"] = std::bind(&rsingle_search_index::do_check_rsingle_ge, _rsingle_index, _1, _2, _3);
    _search_index_map["rsingle_diff_1_ge"] = std::bind(&rsingle_search_index::do_check_rsingle_ge, _rsingle_index, _1, _2, _3);
    _search_index_map["rsingle_diff_2_ge"] = std::bind(&rsingle_search_index::do_check_rsingle_ge, _rsingle_index, _1, _2, _3);
    _search_index_map["rsingle_diff_0_ge_v"] = std::bind(&rsingle_search_index::do_check_rsingle_ge, _rsingle_index, _1, _2, _3);
    _search_index_map["rsingle_diff_1_ge_v"] = std::bind(&rsingle_search_index::do_check_rsingle_ge, _rsingle_index, _1, _2, _3);
    _search_index_map["rsingle_diff_2_ge_v"] = std::bind(&rsingle_search_index::do_check_rsingle_ge, _rsingle_index, _1, _2, _3);

    _search_index_map["rsingle_diff_0_le"] = std::bind(&rsingle_search_index::do_check_rsingle_le, _rsingle_index, _1, _2, _3);
    _search_index_map["rsingle_diff_1_le"] = std::bind(&rsingle_search_index::do_check_rsingle_le, _rsingle_index, _1, _2, _3);
    _search_index_map["rsingle_diff_2_le"] = std::bind(&rsingle_search_index::do_check_rsingle_le, _rsingle_index, _1, _2, _3);
    _search_index_map["rsingle_diff_0_le_v"] = std::bind(&rsingle_search_index::do_check_rsingle_le, _rsingle_index, _1, _2, _3);
    _search_index_map["rsingle_diff_1_le_v"] = std::bind(&rsingle_search_index::do_check_rsingle_le, _rsingle_index, _1, _2, _3);
    _search_index_map["rsingle_diff_2_le_v"] = std::bind(&rsingle_search_index::do_check_rsingle_le, _rsingle_index, _1, _2, _3);

    _search_index_map["rsingle_diff2_0_ge"] = std::bind(&rsingle_search_index::do_check_rsingle_diff2_ge, _rsingle_index, _1, _2, _3);
    _search_index_map["rsingle_diff2_1_ge"] = std::bind(&rsingle_search_index::do_check_rsingle_diff2_ge, _rsingle_index, _1, _2, _3);
    _search_index_map["rsingle_diff2_2_ge"] = std::bind(&rsingle_search_index::do_check_rsingle_diff2_ge, _rsingle_index, _1, _2, _3);
    _search_index_map["rsingle_diff2_0_ge_v"] = std::bind(&rsingle_search_index::do_check_rsingle_diff2_ge, _rsingle_index, _1, _2, _3);
    _search_index_map["rsingle_diff2_1_ge_v"] = std::bind(&rsingle_search_index::do_check_rsingle_diff2_ge, _rsingle_index, _1, _2, _3);
    _search_index_map["rsingle_diff2_2_ge_v"] = std::bind(&rsingle_search_index::do_check_rsingle_diff2_ge, _rsingle_index, _1, _2, _3);

    _search_index_map["rsingle_diff2_0_le"] = std::bind(&rsingle_search_index::do_check_rsingle_diff2_le, _rsingle_index, _1, _2, _3);
    _search_index_map["rsingle_diff2_1_le"] = std::bind(&rsingle_search_index::do_check_rsingle_diff2_le, _rsingle_index, _1, _2, _3);
    _search_index_map["rsingle_diff2_2_le"] = std::bind(&rsingle_search_index::do_check_rsingle_diff2_le, _rsingle_index, _1, _2, _3);
    _search_index_map["rsingle_diff2_0_le_v"] = std::bind(&rsingle_search_index::do_check_rsingle_diff2_le, _rsingle_index, _1, _2, _3);
    _search_index_map["rsingle_diff2_1_le_v"] = std::bind(&rsingle_search_index::do_check_rsingle_diff2_le, _rsingle_index, _1, _2, _3);
    _search_index_map["rsingle_diff2_2_le_v"] = std::bind(&rsingle_search_index::do_check_rsingle_diff2_le, _rsingle_index, _1, _2, _3);

    _hsingle_index = std::make_shared<hsingle_search_index>();
    _search_index_map["hsingle_diff_ge"] = std::bind(&hsingle_search_index::do_hsingle_diff_ge, _hsingle_index, _1, _2, _3);
    _search_index_map["hsingle_diff_le"] = std::bind(&hsingle_search_index::do_hsingle_diff_le, _hsingle_index, _1, _2, _3);
    _search_index_map["hsingle_diff_ge_v"] = std::bind(&hsingle_search_index::do_hsingle_diff_ge, _hsingle_index, _1, _2, _3);
    _search_index_map["hsingle_diff_le_v"] = std::bind(&hsingle_search_index::do_hsingle_diff_le, _hsingle_index, _1, _2, _3);

    _search_index_map["hsingle_diff_ge_num_ge"] = std::bind(&hsingle_search_index::do_hsingle_diff_ge_num_ge, _hsingle_index, _1, _2, _3);
    _search_index_map["hsingle_diff_ge_num_ge_v"] = std::bind(&hsingle_search_index::do_hsingle_diff_ge_num_ge, _hsingle_index, _1, _2, _3);

    _search_index_map["hsingle_diff_sum_ge"] = std::bind(&hsingle_search_index::do_hsingle_sum_ge, _hsingle_index, _1, _2, _3);
    _search_index_map["hsingle_diff_sum_ge_v"] = std::bind(&hsingle_search_index::do_hsingle_sum_ge, _hsingle_index, _1, _2, _3);

    _search_index_map["hsingle_diff_sum_le"] = std::bind(&hsingle_search_index::do_hsingle_sum_le, _hsingle_index, _1, _2, _3);
    _search_index_map["hsingle_diff_sum_le_v"] = std::bind(&hsingle_search_index::do_hsingle_sum_le, _hsingle_index, _1, _2, _3);

    _hquotation_index = std::make_shared<hquotation_search_index>();

    _search_index_map["hqchange_rate_ge"] = std::bind(&hquotation_search_index::do_hqchange_rate_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["hqchange_rate_ge_v"] = std::bind(&hquotation_search_index::do_hqchange_rate_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["hqchange_rate_le"] = std::bind(&hquotation_search_index::do_hqchange_rate_le, _hquotation_index, _1, _2, _3);
    _search_index_map["hqchange_rate_le_v"] = std::bind(&hquotation_search_index::do_hqchange_rate_le, _hquotation_index, _1, _2, _3);

    _search_index_map["hqrange_percent_ge"] = std::bind(&hquotation_search_index::do_hqrange_percent_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["hqrange_percent_ge_v"] = std::bind(&hquotation_search_index::do_hqrange_percent_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["hqrange_percent_le"] = std::bind(&hquotation_search_index::do_hqrange_percent_le, _hquotation_index, _1, _2, _3);
    _search_index_map["hqrange_percent_le_v"] = std::bind(&hquotation_search_index::do_hqrange_percent_le, _hquotation_index, _1, _2, _3);

    _search_index_map["hqrange_percent_ge_num_ge"] = std::bind(&hquotation_search_index::do_hqrange_percent_ge_num_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["hqrange_percent_ge_num_ge_v"] = std::bind(&hquotation_search_index::do_hqrange_percent_ge_num_ge, _hquotation_index, _1, _2, _3);

    _search_index_map["hq_sum_range_percent_ge"] = std::bind(&hquotation_search_index::do_hq_sum_range_percent_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["hq_sum_range_percent_ge_v"] = std::bind(&hquotation_search_index::do_hq_sum_range_percent_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["hq_sum_range_percent_le"] = std::bind(&hquotation_search_index::do_hq_sum_range_percent_le, _hquotation_index, _1, _2, _3);
    _search_index_map["hq_sum_range_percent_le_v"] = std::bind(&hquotation_search_index::do_hq_sum_range_percent_le, _hquotation_index, _1, _2, _3);

    _search_index_map["hq_sum_change_rate_ge"] = std::bind(&hquotation_search_index::do_hq_sum_change_rate_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["hq_sum_change_rate_ge_v"] = std::bind(&hquotation_search_index::do_hq_sum_change_rate_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["hq_sum_change_rate_le"] = std::bind(&hquotation_search_index::do_hq_sum_change_rate_le, _hquotation_index, _1, _2, _3);
    _search_index_map["hq_sum_change_rate_le_v"] = std::bind(&hquotation_search_index::do_hq_sum_change_rate_le, _hquotation_index, _1, _2, _3);

    _search_index_map["hqdown_pointer_ge"] = std::bind(&hquotation_search_index::do_check_hqdown_pointer_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["hqdown_pointer_ge_v"] = std::bind(&hquotation_search_index::do_check_hqdown_pointer_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["hqdown_pointer_le"] = std::bind(&hquotation_search_index::do_check_hqdown_pointer_le, _hquotation_index, _1, _2, _3);
    _search_index_map["hqdown_pointer_le_v"] = std::bind(&hquotation_search_index::do_check_hqdown_pointer_le, _hquotation_index, _1, _2, _3);
    _search_index_map["hqdown_pointer_ge_num_ge"] = std::bind(&hquotation_search_index::do_check_hqdown_pointer_ge_num_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["hqdown_pointer_ge_num_ge_v"] = std::bind(&hquotation_search_index::do_check_hqdown_pointer_ge_num_ge, _hquotation_index, _1, _2, _3);

    _search_index_map["hqup_pointer_ge"] = std::bind(&hquotation_search_index::do_check_hqup_pointer_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["hqup_pointer_ge_v"] = std::bind(&hquotation_search_index::do_check_hqup_pointer_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["hqup_pointer_le"] = std::bind(&hquotation_search_index::do_check_hqup_pointer_le, _hquotation_index, _1, _2, _3);
    _search_index_map["hqup_pointer_le_v"] = std::bind(&hquotation_search_index::do_check_hqup_pointer_le, _hquotation_index, _1, _2, _3);
    _search_index_map["hqup_pointer_ge_num_ge"] = std::bind(&hquotation_search_index::do_check_hqup_pointer_ge_num_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["hqup_pointer_ge_num_ge_v"] = std::bind(&hquotation_search_index::do_check_hqup_pointer_ge_num_ge, _hquotation_index, _1, _2, _3);

    _search_index_map["hqend_start_ge"] = std::bind(&hquotation_search_index::do_check_hqend_start_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["hqend_start_ge_v"] = std::bind(&hquotation_search_index::do_check_hqend_start_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["hqend_start_le"] = std::bind(&hquotation_search_index::do_check_hqend_start_le, _hquotation_index, _1, _2, _3);
    _search_index_map["hqend_start_le_v"] = std::bind(&hquotation_search_index::do_check_hqend_start_le, _hquotation_index, _1, _2, _3);
    _search_index_map["hqend_start_ge_num_ge"] = std::bind(&hquotation_search_index::do_check_hqend_start_ge_num_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["hqend_start_ge_num_ge_v"] = std::bind(&hquotation_search_index::do_check_hqend_start_ge_num_ge, _hquotation_index, _1, _2, _3);

    _search_index_map["hqend_avg_end_ge"] = std::bind(&hquotation_search_index::do_check_hqend_avg_end_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["hqend_avg_end_ge_v"] = std::bind(&hquotation_search_index::do_check_hqend_avg_end_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["hqend_avg_end_le"] = std::bind(&hquotation_search_index::do_check_hqend_avg_end_le, _hquotation_index, _1, _2, _3);
    _search_index_map["hqend_avg_end_le_v"] = std::bind(&hquotation_search_index::do_check_hqend_avg_end_le, _hquotation_index, _1, _2, _3);
    _search_index_map["hqend_avg_end_ge_num_ge"] = std::bind(&hquotation_search_index::do_check_hqend_avg_end_ge_num_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["hqend_avg_end_ge_num_ge_v"] = std::bind(&hquotation_search_index::do_check_hqend_avg_end_ge_num_ge, _hquotation_index, _1, _2, _3);

    _search_index_map["id_substr"] = std::bind(&hquotation_search_index::do_check_id_substr, _hquotation_index, _1, _2, _3);
    _search_index_map["id_substr_v"] = std::bind(&hquotation_search_index::do_check_id_substr, _hquotation_index, _1, _2, _3);


    _search_index_map["rlow_hlow_ge"] = std::bind(&hquotation_search_index::do_check_rlow_hlow_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["rlow_hlow_ge_v"] = std::bind(&hquotation_search_index::do_check_rlow_hlow_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["rlow_hlow_le"] = std::bind(&hquotation_search_index::do_check_rlow_hlow_le, _hquotation_index, _1, _2, _3);
    _search_index_map["rlow_hlow_le_v"] = std::bind(&hquotation_search_index::do_check_rlow_hlow_le, _hquotation_index, _1, _2, _3);
    _search_index_map["rlow_hlow_ge_num_ge"] = std::bind(&hquotation_search_index::do_check_rlow_hlow_ge_num_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["rlow_hlow_ge_num_ge_v"] = std::bind(&hquotation_search_index::do_check_rlow_hlow_ge_num_ge, _hquotation_index, _1, _2, _3);

    _search_index_map["rhigh_hhigh_ge"] = std::bind(&hquotation_search_index::do_check_rhigh_hhigh_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["rhigh_hhigh_ge_v"] = std::bind(&hquotation_search_index::do_check_rhigh_hhigh_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["rhigh_hhigh_le"] = std::bind(&hquotation_search_index::do_check_rhigh_hhigh_le, _hquotation_index, _1, _2, _3);
    _search_index_map["rhigh_hhigh_le_v"] = std::bind(&hquotation_search_index::do_check_rhigh_hhigh_le, _hquotation_index, _1, _2, _3);
    _search_index_map["rhigh_hhigh_ge_num_ge"] = std::bind(&hquotation_search_index::do_check_rhigh_hhigh_ge_num_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["rhigh_hhigh_ge_num_ge_v"] = std::bind(&hquotation_search_index::do_check_rhigh_hhigh_ge_num_ge, _hquotation_index, _1, _2, _3);


    _search_index_map["hqend_end_5_ge"] = std::bind(&hquotation_search_index::do_check_hqend_end_5_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["hqend_end_5_ge_v"] = std::bind(&hquotation_search_index::do_check_hqend_end_5_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["hqend_end_5_le"] = std::bind(&hquotation_search_index::do_check_hqend_end_5_le, _hquotation_index, _1, _2, _3);
    _search_index_map["hqend_end_5_le_v"] = std::bind(&hquotation_search_index::do_check_hqend_end_5_le, _hquotation_index, _1, _2, _3);
    _search_index_map["hqend_end_5_ge_num_ge"] = std::bind(&hquotation_search_index::do_check_hqend_end_5_ge_num_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["hqend_end_5_ge_num_ge_v"] = std::bind(&hquotation_search_index::do_check_hqend_end_5_ge_num_ge, _hquotation_index, _1, _2, _3);

    _search_index_map["hqend_end_10_ge"] = std::bind(&hquotation_search_index::do_check_hqend_end_10_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["hqend_end_10_ge_v"] = std::bind(&hquotation_search_index::do_check_hqend_end_10_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["hqend_end_10_le"] = std::bind(&hquotation_search_index::do_check_hqend_end_10_le, _hquotation_index, _1, _2, _3);
    _search_index_map["hqend_end_10_le_v"] = std::bind(&hquotation_search_index::do_check_hqend_end_10_le, _hquotation_index, _1, _2, _3);
    _search_index_map["hqend_end_10_ge_num_ge"] = std::bind(&hquotation_search_index::do_check_hqend_end_10_ge_num_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["hqend_end_10_ge_num_ge_v"] = std::bind(&hquotation_search_index::do_check_hqend_end_10_ge_num_ge, _hquotation_index, _1, _2, _3);

    _search_index_map["hqend_end_20_ge"] = std::bind(&hquotation_search_index::do_check_hqend_end_20_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["hqend_end_20_ge_v"] = std::bind(&hquotation_search_index::do_check_hqend_end_20_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["hqend_end_20_le"] = std::bind(&hquotation_search_index::do_check_hqend_end_20_le, _hquotation_index, _1, _2, _3);
    _search_index_map["hqend_end_20_le_v"] = std::bind(&hquotation_search_index::do_check_hqend_end_20_le, _hquotation_index, _1, _2, _3);
    _search_index_map["hqend_end_20_ge_num_ge"] = std::bind(&hquotation_search_index::do_check_hqend_end_20_ge_num_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["hqend_end_20_ge_num_ge_v"] = std::bind(&hquotation_search_index::do_check_hqend_end_20_ge_num_ge, _hquotation_index, _1, _2, _3);

    _search_index_map["hqend_end_30_ge"] = std::bind(&hquotation_search_index::do_check_hqend_end_30_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["hqend_end_30_ge_v"] = std::bind(&hquotation_search_index::do_check_hqend_end_30_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["hqend_end_30_le"] = std::bind(&hquotation_search_index::do_check_hqend_end_30_le, _hquotation_index, _1, _2, _3);
    _search_index_map["hqend_end_30_le_v"] = std::bind(&hquotation_search_index::do_check_hqend_end_30_le, _hquotation_index, _1, _2, _3);
    _search_index_map["hqend_end_30_ge_num_ge"] = std::bind(&hquotation_search_index::do_check_hqend_end_30_ge_num_ge, _hquotation_index, _1, _2, _3);
    _search_index_map["hqend_end_30_ge_num_ge_v"] = std::bind(&hquotation_search_index::do_check_hqend_end_30_ge_num_ge, _hquotation_index, _1, _2, _3);
}

base_search_index proc_data::get_search_index(std::string & key)
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
