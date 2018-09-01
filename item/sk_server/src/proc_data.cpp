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
#include "recommend_dict.h"

int proc_data::init(sk_conf * conf)
{
    get_proc_name(proc_name, sizeof(proc_name));

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
        recommend_dict *recommend_dict1 = new (std::nothrow)recommend_dict();
        ASSERT_WARNING(recommend_dict1 != NULL, "allocate recommend_dict fail");
        recommend_dict1->init(_conf->_strategy->current()->recommend_dict_path.c_str(), 
                _conf->_strategy->current()->recommend_dict_file.c_str(), _conf->dump_dir.c_str());

        recommend_dict *recommend_dict2 = new (std::nothrow)recommend_dict();
        ASSERT_WARNING(recommend_dict2 != NULL, "allocate recommend_dict fail");
        recommend_dict2->init(_conf->_strategy->current()->recommend_dict_path.c_str(),
                _conf->_strategy->current()->recommend_dict_file.c_str(), _conf->dump_dir.c_str());

        _recommend_dict = new (std::nothrow)reload_mgr<recommend_dict>(recommend_dict1, recommend_dict2);
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
    if (_ua_dict)
    {
        _ua_dict->reload();
    }

    if (_id_dict)
    {
        _id_dict->reload();
    }

    _finance_dict.reload();

    if (_ban_dict)
    {
        _ban_dict->reload();
    }

    if (_addr_dict_split)
    {
        _addr_dict_split->reload();
    }

    if (_addr_dict)
    {
        _addr_dict->reload();
    }

    if (_plate_dict_split)
    {
        _plate_dict_split->reload();
    }

    if (_plate_dict)
    {
        _plate_dict->reload();
    }

    if (_hsingle_dict)
    {
        _hsingle_dict->reload();
    }

    if (_hquoation_dict)
    {
        _hquoation_dict->reload();
    }

    if (_holiday_dict)
    {
        _holiday_dict->reload();
    }
    return 0;
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
    _ua_dict->destroy_idle();

    _id_dict->destroy_idle();

    _finance_dict.destroy_idle();

    _ban_dict->destroy_idle();

    _addr_dict_split->destroy_idle();

    _addr_dict->destroy_idle();

    _plate_dict_split->destroy_idle();

    _plate_dict->destroy_idle();

    _hsingle_dict->destroy_idle();

    _hquoation_dict->destroy_idle();

    _holiday_dict->destroy_idle();

    return 0;
}
