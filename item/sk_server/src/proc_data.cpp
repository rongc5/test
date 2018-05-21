#include "log_helper.h"
#include "ua_dict.h"
#include "strategy_dict.h"
#include "id_dict.h"
#include "finance_dict.h"
#include "ban_dict.h"
#include "real_single_dict.h"
#include "real_quotation_dict.h"
#include "proc_data.h"
#include "address_dict.h"
#include "plate_dict.h"
#include "history_single_dict.h"
#include "history_quotation_dict.h"
#include "trade_date_dict.h"

int proc_data::init(sk_conf * conf)
{
    _conf = conf;
    
    {
        ua_dict *ua_dict1 = new (std::nothrow)ua_dict();
        ASSERT_WARNING(ua_dict1 != NULL, "allocate ua_dict fail");
        ua_dict1->init(_conf->ua_path.c_str(), _conf->ua_file.c_str(), _conf->dump_dir.c_str());

        ua_dict *ua_dict2 = new (std::nothrow)ua_dict();
        ASSERT_WARNING(ua_dict2 != NULL, "allocate ua_dict fail");
        ua_dict2->init(_conf->ua_path.c_str(), _conf->ua_file.c_str(), _conf->dump_dir.c_str());

        _ua_dict = new (std::nothrow)reload_mgr<ua_dict>(ua_dict1, ua_dict2);
    }

    {
        id_dict *id_dict1 = new (std::nothrow)id_dict();
        ASSERT_WARNING(id_dict1 != NULL, "allocate id_dict fail");
        id_dict1->init(_conf->id_path.c_str(), _conf->id_file.c_str(), _conf->dump_dir.c_str());

        id_dict *id_dict2 = new (std::nothrow)id_dict();
        ASSERT_WARNING(id_dict2 != NULL, "allocate id_dict fail");
        id_dict2->init(_conf->id_path.c_str(), _conf->id_file.c_str(), _conf->dump_dir.c_str());

        _id_dict = new (std::nothrow)reload_mgr<id_dict>(id_dict1, id_dict2);
    }

    {
        real_quotation_dict *rq_dict1 = new (std::nothrow)real_quotation_dict();
        ASSERT_WARNING(rq_dict1 != NULL, "allocate id_dict fail");
        rq_dict1->init(_conf->id_path.c_str(), _conf->id_file.c_str(), _conf->dump_dir.c_str());

        real_quotation_dict *rq_dict2 = new (std::nothrow)real_quotation_dict();
        ASSERT_WARNING(rq_dict2 != NULL, "allocate id_dict fail");
        rq_dict2->init(_conf->id_path.c_str(), _conf->id_file.c_str(), _conf->dump_dir.c_str());

        _rquoation_dict = new (std::nothrow)reload_mgr<real_quotation_dict>(rq_dict1, rq_dict2);
    }

    {
        real_single_dict *rs_dict1 = new (std::nothrow)real_single_dict();
        ASSERT_WARNING(rs_dict1 != NULL, "allocate id_dict fail");
        rs_dict1->init(_conf->id_path.c_str(), _conf->id_file.c_str(), _conf->dump_dir.c_str());

        real_single_dict *rs_dict2 = new (std::nothrow)real_single_dict();
        ASSERT_WARNING(rs_dict2 != NULL, "allocate id_dict fail");
        rs_dict2->init(_conf->id_path.c_str(), _conf->id_file.c_str(), _conf->dump_dir.c_str());

        _rsingle_dict = new (std::nothrow)reload_mgr<real_single_dict>(rs_dict1, rs_dict2);
    }


    {
        strategy_dict *strategy_dict1 = new (std::nothrow)strategy_dict();
        ASSERT_WARNING(strategy_dict1 != NULL, "allocate strategy_dict fail");
        strategy_dict1->init(_conf->local_strategy_path.c_str(), _conf->local_strategy_file.c_str(), _conf->dump_dir.c_str());

        strategy_dict *strategy_dict2 = new (std::nothrow)strategy_dict();
        ASSERT_WARNING(strategy_dict2 != NULL, "allocate strategy_dict fail");
        strategy_dict2->init(_conf->local_strategy_path.c_str(), _conf->local_strategy_file.c_str(), _conf->dump_dir.c_str());

        _strategy_dict = new (std::nothrow)reload_mgr<strategy_dict>(strategy_dict1, strategy_dict2);
    }


    {
        address_dict *addr_dict1 = new (std::nothrow)address_dict();
        ASSERT_WARNING(addr_dict1 != NULL, "allocate address_dict fail");
        addr_dict1->init(_conf->address_dict_path.c_str(), _conf->address_dict_file.c_str(), _conf->dump_dir.c_str());

        address_dict *addr_dict2 = new (std::nothrow)address_dict();
        ASSERT_WARNING(addr_dict2 != NULL, "allocate address_dict fail");
        addr_dict2->init(_conf->address_dict_path.c_str(), _conf->address_dict_file.c_str(), _conf->dump_dir.c_str());

        _address_dict = new (std::nothrow)reload_mgr<address_dict>(addr_dict1, addr_dict2);
    }


    {
        plate_dict *addr_dict1 = new (std::nothrow)plate_dict();
        ASSERT_WARNING(addr_dict1 != NULL, "allocate plate_dict fail");
        addr_dict1->init(_conf->plate_dict_path.c_str(), _conf->plate_dict_file.c_str(), _conf->dump_dir.c_str());

        plate_dict *addr_dict2 = new (std::nothrow)plate_dict();
        ASSERT_WARNING(addr_dict2 != NULL, "allocate plate_dict fail");
        addr_dict2->init(_conf->plate_dict_path.c_str(), _conf->plate_dict_file.c_str(), _conf->dump_dir.c_str());

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
        finance_dict *fin_dict1 = new (std::nothrow)finance_dict();
        ASSERT_WARNING(fin_dict1 != NULL, "allocate finance_dict fail");
        fin_dict1->init(_conf->financie_path.c_str(), _conf->financie_file.c_str(), conf->dump_dir.c_str());

        finance_dict *fin_dict2 = new (std::nothrow)finance_dict();
        ASSERT_WARNING(fin_dict2 != NULL, "allocate finance_dict fail");
        fin_dict2->init(_conf->financie_path.c_str(), conf->financie_file.c_str(), conf->dump_dir.c_str());

        _finance_dict = new (std::nothrow)reload_mgr<finance_dict>(fin_dict1, fin_dict2);
    }


    {
        ban_dict *b_dict1 = new (std::nothrow)ban_dict();
        ASSERT_WARNING(b_dict1 != NULL, "allocate ban_dict fail");
        b_dict1->init(_conf->ban_path.c_str(), _conf->ban_file.c_str(), conf->dump_dir.c_str());

        ban_dict *b_dict2 = new (std::nothrow)ban_dict();
        ASSERT_WARNING(b_dict2 != NULL, "allocate ban_dict fail");
        b_dict2->init(_conf->ban_path.c_str(), _conf->ban_file.c_str(), conf->dump_dir.c_str());

        _ban_dict = new (std::nothrow)reload_mgr<ban_dict>(b_dict1, b_dict2);
    }
    
    {
        history_single_dict *hs_dict1 = new (std::nothrow)history_single_dict();
        ASSERT_WARNING(hs_dict1 != NULL, "allocate history_single_t fail");
        hs_dict1->init(_conf->history_single_path.c_str(), _conf->history_single_file.c_str(), conf->dump_dir.c_str());

        history_single_dict *hs_dict2 = new (std::nothrow)history_single_dict();
        ASSERT_WARNING(hs_dict2 != NULL, "allocate history_single_t fail");
        hs_dict2->init(_conf->history_single_path.c_str(), _conf->history_single_file.c_str(), conf->dump_dir.c_str());

        _hsingle_dict = new (std::nothrow)reload_mgr<history_single_dict>(hs_dict1, hs_dict2);
    }


    {
        history_quotation_dict *hq_dict1 = new (std::nothrow)history_quotation_dict();
        ASSERT_WARNING(hq_dict1 != NULL, "allocate history_quotation_t fail");
        hq_dict1->init(_conf->history_quotation_path.c_str(), _conf->history_quotation_file.c_str(), conf->dump_dir.c_str());

        history_quotation_dict *hq_dict2 = new (std::nothrow)history_quotation_dict();
        ASSERT_WARNING(hq_dict2 != NULL, "allocate history_quotation_t fail");
        hq_dict2->init(_conf->history_quotation_path.c_str(), _conf->history_quotation_file.c_str(), conf->dump_dir.c_str());

        _hquoation_dict = new (std::nothrow)reload_mgr<history_quotation_dict>(hq_dict1, hq_dict2);
    }

    {
        trade_date_dict *td_dict1 = new (std::nothrow)trade_date_dict();
        ASSERT_WARNING(td_dict1 != NULL, "allocate trade_date_dict fail");
        td_dict1->init(_conf->trade_date_path.c_str(), _conf->trade_date_file.c_str(), conf->dump_dir.c_str());

        trade_date_dict *td_dict2 = new (std::nothrow)trade_date_dict();
        ASSERT_WARNING(td_dict2 != NULL, "allocate trade_date_dict fail");
        td_dict2->init(_conf->trade_date_path.c_str(), _conf->trade_date_file.c_str(), conf->dump_dir.c_str());

        _tdate_dict = new (std::nothrow)reload_mgr<trade_date_dict>(td_dict1, td_dict2);

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

    _strategy_dict->load();

    _finance_dict->load();

    _ban_dict->load();

    _rsingle_dict->load();

    _rquoation_dict->load();

    _address_dict->load();

    _plate_dict->load();

    _hsingle_dict->load();

    _hquoation_dict->load();

    _tdate_dict->load();

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

    if (_strategy_dict)
    {
        _strategy_dict->reload();
    }

    if (_finance_dict)
    {
        _finance_dict->reload();
    }

    if (_ban_dict)
    {
        _ban_dict->reload();
    }

    if (_rsingle_dict)
    {
        _rsingle_dict->reload();
    }

    if (_rquoation_dict)
    {
        _rquoation_dict->reload();
    }

    if (_address_dict)
    {
        _address_dict->reload();
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

    if (_tdate_dict)
    {
        _tdate_dict->reload();
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

    _strategy_dict->dump();

    _finance_dict->dump();

    _ban_dict->dump();

    _rsingle_dict->dump();

    _rquoation_dict->dump();

    _address_dict->dump();

    _plate_dict->dump();

    _hsingle_dict->dump();

    _hquoation_dict->dump();

    _tdate_dict->dump();

    return 0;
}
int proc_data::destroy()
{
    _ua_dict->destroy();

    _id_dict->destroy();

    _strategy_dict->destroy();

    _finance_dict->destroy();

    _ban_dict->destroy();

    _rsingle_dict->destroy();

    _rquoation_dict->destroy();

    _address_dict->destroy();

    _plate_dict->destroy();

    _hsingle_dict->destroy();

    _hquoation_dict->destroy();

    _tdate_dict->destroy();

    return 0;
}
