#include "strategy_conf.h"
#include "base_def.h"
#include "log_helper.h"
#include "common_util.h"

strategy_conf::strategy_conf()
{
    req_quotation_millisecond = 0;
    req_single_millisecond = 0;
    req_main_funds_millisecond = 0;
    req_http_timeout = 0;
    reload_millisecond = 0;
    max_reqhttp_num = 0;
    is_req_single_from_file = 0;
    is_req_main_funds_from_file = 0;
    req_single_num_permillisecond = 0;
    req_main_funds_num_permillisecond = 0;
    req_single_num_perreq = 0;
    real_single_deque_length = 0;
    
    is_req_quotation_from_file = 0;
    req_quotation_num_permillisecond = 0;
    req_quotation_num_perreq = 0;
    real_quotation_deque_length = 0;
    history_quotation_num = 0;
    history_technical_num = 0;
    history_single_num = 0;
    history_main_funds_num = 0;
    history_wquotation_num =0;
    
    history_wsingle_num = 0;
    destroy_idle_millisecond = 0;
    per_day_min_req_circle_times = 0;
    per_day_perdump_with_circles = 0;
    log_single_deque_length = 0;
    log_main_funds_deque_length = 0;
    log_single_deque_length_max = 0;
    log_main_funds_deque_length_max = 0;
    lru_ssr_length = 0;
    is_not_trade_date_ratio = 0;

}

strategy_conf::~strategy_conf()
{
    destroy();
}

int strategy_conf::init(const char * path, const char * file, const char *dump_dir)
{
    snprintf(_fullpath, sizeof(_fullpath), "%s/%s", path, file);
    snprintf(_dumppath, sizeof(_dumppath), "%s/%s", dump_dir, file);

    destroy();

    return 0;
}

int strategy_conf::load()
{
    FILE * fp = fopen(_fullpath, "r");
    ASSERT_WARNING(fp != NULL,"open query dict failed. path[%s]", _fullpath);

    char line[SIZE_LEN_1024];
    char * ptr = NULL;
    
    _cfg.clear();
    vector<string> tmp_vec;
    while (fgets(line, 1024, fp))
    {
        if('\0' == line[0]){
            continue;
        }

        ptr = im_chomp(line);
        ptr = im_chomp(ptr, '#');

        if (ptr == NULL || *ptr == '\0'|| *ptr == '#')
            continue;
        
        SplitString(ptr, "=", &tmp_vec, SPLIT_MODE_ONE | SPLIT_MODE_TRIM);
        if (tmp_vec.size() != 2)
            continue;

        _cfg.insert(make_pair(trim(tmp_vec[0].c_str()), trim(tmp_vec[1].c_str())));
    }
    fclose(fp);

    do_parse();

    struct stat st;
    stat(_fullpath, &st);
    _last_load = st.st_mtime;

    return 0;
}

void strategy_conf::do_parse()
{
    if (has_key<string, string>(_cfg, "real_morning_stime"))
    {
        real_morning_stime = _cfg["real_morning_stime"];
    }

    if (has_key<string, string>(_cfg, "backup_stime"))
    {
        backup_stime = _cfg["backup_stime"];
    }

    if (has_key<string, string>(_cfg, "real_morning_etime"))
    {
        real_morning_etime = _cfg["real_morning_etime"];
    }

    if (has_key<string, string>(_cfg, "real_afternoon_stime"))
    {
        real_afternoon_stime = _cfg["real_afternoon_stime"];
    }

    if (has_key<string, string>(_cfg, "real_afternoon_etime"))
    {
        real_afternoon_etime = _cfg["real_afternoon_etime"];
    }

    if (has_key<string, string>(_cfg, "dump_real_time"))
    {
        dump_real_time = _cfg["dump_real_time"];
    }

    if (has_key<string, string>(_cfg, "cafile"))
    {
        cafile = _cfg["cafile"];
    }

    if (has_key<string, string>(_cfg, "req_http_timeout"))
    {
        req_http_timeout = atoi(_cfg["req_http_timeout"].c_str());
    }

    if (has_key<string, string>(_cfg, "req_quotation_millisecond"))
    {
        req_quotation_millisecond = atoi(_cfg["req_quotation_millisecond"].c_str());
    }

    if (has_key<string, string>(_cfg, "destroy_idle_millisecond"))
    {
        destroy_idle_millisecond = atoi(_cfg["destroy_idle_millisecond"].c_str());
    }

    if (has_key<string, string>(_cfg, "req_single_millisecond"))
    {
        req_single_millisecond = atoi(_cfg["req_single_millisecond"].c_str());
    }
    if (has_key<string, string>(_cfg, "req_main_funds_millisecond"))
    {
        req_main_funds_millisecond = atoi(_cfg["req_main_funds_millisecond"].c_str());
    }

    if (has_key<string, string>(_cfg, "req_single_num_permillisecond"))
    {
        req_single_num_permillisecond = atoi(_cfg["req_single_num_permillisecond"].c_str());
    }
    if (has_key<string, string>(_cfg, "req_main_funds_num_permillisecond"))
    {
        req_main_funds_num_permillisecond = atoi(_cfg["req_main_funds_num_permillisecond"].c_str());
    }


    if (has_key<string, string>(_cfg, "req_single_num_perreq"))
    {
        req_single_num_perreq = atoi(_cfg["req_single_num_perreq"].c_str());
    }


    if (has_key<string, string>(_cfg, "req_quotation_num_perreq"))
    {
        req_quotation_num_perreq = atoi(_cfg["req_quotation_num_perreq"].c_str());
    }


    if (has_key<string, string>(_cfg, "req_quotation_num_permillisecond"))
    {
        req_quotation_num_permillisecond = atoi(_cfg["req_quotation_num_permillisecond"].c_str());
    }


    if (has_key<string, string>(_cfg, "reload_millisecond"))
    {
        reload_millisecond = atoi(_cfg["reload_millisecond"].c_str());
    }

    if (has_key<string, string>(_cfg, "ua_path"))
    {
        ua_path = _cfg["ua_path"];
    }

    if (has_key<string, string>(_cfg, "ua_file"))
    {
        ua_file = _cfg["ua_file"];
    }

    if (has_key<string, string>(_cfg, "ua_path"))
    {
        ua_path = _cfg["ua_path"];
    }

    if (has_key<string, string>(_cfg, "id_path"))
    {
        id_path = _cfg["id_path"];
    }

    if (has_key<string, string>(_cfg, "id_file"))
    {
        id_file = _cfg["id_file"];
    }

    if (has_key<string, string>(_cfg, "financie_path"))
    {
        financie_path = _cfg["financie_path"];
    }

    if (has_key<string, string>(_cfg, "financie_file"))
    {
        financie_file = _cfg["financie_file"];
    }

    if (has_key<string, string>(_cfg, "ban_path"))
    {
        ban_path = _cfg["ban_path"];
    }

    if (has_key<string, string>(_cfg, "ban_file"))
    {
        ban_file = _cfg["ban_file"];
    }

    if (has_key<string, string>(_cfg, "plate_dict_split_path"))
    {
        plate_dict_split_path = _cfg["plate_dict_split_path"];
    }

    if (has_key<string, string>(_cfg, "plate_dict_split_file"))
    {
        plate_dict_split_file = _cfg["plate_dict_split_file"];
    }

    if (has_key<string, string>(_cfg, "plate_dict_path"))
    {
        plate_dict_path = _cfg["plate_dict_path"];
    }

    if (has_key<string, string>(_cfg, "plate_dict_file"))
    {
        plate_dict_file = _cfg["plate_dict_file"];
    }


    if (has_key<string, string>(_cfg, "cir_holder_dict_split_path"))
    {
        cir_holder_dict_split_path = _cfg["cir_holder_dict_split_path"];
    }

    if (has_key<string, string>(_cfg, "cir_holder_dict_split_file"))
    {
        cir_holder_dict_split_file = _cfg["cir_holder_dict_split_file"];
    }

    if (has_key<string, string>(_cfg, "cir_holder_dict_path"))
    {
        cir_holder_dict_path = _cfg["cir_holder_dict_path"];
    }

    if (has_key<string, string>(_cfg, "cir_holder_dict_file"))
    {
        cir_holder_dict_file = _cfg["cir_holder_dict_file"];
    }


    if (has_key<string, string>(_cfg, "addr_dict_split_path"))
    {
        addr_dict_split_path = _cfg["addr_dict_split_path"];
    }

    if (has_key<string, string>(_cfg, "addr_dict_split_file"))
    {
        addr_dict_split_file = _cfg["addr_dict_split_file"];
    }

    if (has_key<string, string>(_cfg, "addr_dict_path"))
    {
        addr_dict_path = _cfg["addr_dict_path"];
    }

    if (has_key<string, string>(_cfg, "addr_dict_file"))
    {
        addr_dict_file = _cfg["addr_dict_file"];
    }

    if (has_key<string, string>(_cfg, "history_single_path"))
    {
        history_single_path = _cfg["history_single_path"];
    }
    if (has_key<string, string>(_cfg, "history_main_funds_path"))
    {
        history_main_funds_path = _cfg["history_main_funds_path"];
    }

    if (has_key<string, string>(_cfg, "history_single_file"))
    {
        history_single_file = _cfg["history_single_file"];
    }
    if (has_key<string, string>(_cfg, "history_main_funds_file"))
    {
        history_main_funds_file = _cfg["history_main_funds_file"];
    }

    if (has_key<string, string>(_cfg, "history_quotation_path"))
    {
        history_quotation_path = _cfg["history_quotation_path"];
    }

    if (has_key<string, string>(_cfg, "history_quotation_file"))
    {
        history_quotation_file = _cfg["history_quotation_file"];
    }

    if (has_key<string, string>(_cfg, "history_technical_path"))
    {
        history_technical_path = _cfg["history_technical_path"];
    }

    if (has_key<string, string>(_cfg, "history_technical_file"))
    {
        history_technical_file = _cfg["history_technical_file"];
    }


    if (has_key<string, string>(_cfg, "userid_dict_path"))
    {
        userid_dict_path = _cfg["userid_dict_path"];
    }

    if (has_key<string, string>(_cfg, "dump_quotation_schema"))
    {
        dump_quotation_schema = _cfg["dump_quotation_schema"];
    }

    if (has_key<string, string>(_cfg, "dump_wsingle_schema"))
    {
        dump_wsingle_schema = _cfg["dump_wsingle_schema"];
    }

    if (has_key<string, string>(_cfg, "dump_wquotation_schema"))
    {
        dump_wquotation_schema = _cfg["dump_wquotation_schema"];
    }

    if (has_key<string, string>(_cfg, "dump_wtechnical_schema"))
    {
        dump_wtechnical_schema = _cfg["dump_wtechnical_schema"];
    }

    if (has_key<string, string>(_cfg, "dump_single_schema"))
    {
        dump_single_schema = _cfg["dump_single_schema"];
    }
    if (has_key<string, string>(_cfg, "dump_main_funds_schema"))
    {
        dump_main_funds_schema = _cfg["dump_main_funds_schema"];
    }

    if (has_key<string, string>(_cfg, "dump_technical_schema"))
    {
        dump_technical_schema = _cfg["dump_technical_schema"];
    }

    if (has_key<string, string>(_cfg, "dump_wave_schema"))
    {
        dump_wave_schema = _cfg["dump_wave_schema"];
    }
    if (has_key<string, string>(_cfg, "dump_wwave_schema"))
    {
        dump_wwave_schema = _cfg["dump_wwave_schema"];
    }


    if (has_key<string, string>(_cfg, "userid_dict_file"))
    {
        userid_dict_file = _cfg["userid_dict_file"];
    }



    if (has_key<string, string>(_cfg, "history_wsingle_path"))
    {
        history_wsingle_path = _cfg["history_wsingle_path"];
    }

    if (has_key<string, string>(_cfg, "history_wsingle_file"))
    {
        history_wsingle_file = _cfg["history_wsingle_file"];
    }

    if (has_key<string, string>(_cfg, "history_wquotation_path"))
    {
        history_wquotation_path = _cfg["history_wquotation_path"];
    }

    if (has_key<string, string>(_cfg, "history_wquotation_file"))
    {
        history_wquotation_file = _cfg["history_wquotation_file"];
    }


    if (has_key<string, string>(_cfg, "holiday_dict_path"))
    {
        holiday_dict_path = _cfg["holiday_dict_path"];
    }

    if (has_key<string, string>(_cfg, "backup_files_path"))
    {
        backup_files_path = _cfg["backup_files_path"];
    }

    if (has_key<string, string>(_cfg, "backuped_path"))
    {
        backuped_path = _cfg["backuped_path"];
    }

    if (has_key<string, string>(_cfg, "holiday_dict_file"))
    {
        holiday_dict_file = _cfg["holiday_dict_file"];
    }

    if (has_key<string, string>(_cfg, "max_reqhttp_num"))
    {
        max_reqhttp_num = atoi(_cfg["max_reqhttp_num"].c_str());
    }

    if (has_key<string, string>(_cfg, "history_quotation_num"))
    {
        history_quotation_num = atoi(_cfg["history_quotation_num"].c_str());
    }

    if (has_key<string, string>(_cfg, "history_technical_num"))
    {
        history_technical_num = atoi(_cfg["history_technical_num"].c_str());
    }

    if (has_key<string, string>(_cfg, "history_single_num"))
    {
        history_single_num = atoi(_cfg["history_single_num"].c_str());
    }

    if (has_key<string, string>(_cfg, "history_main_funds_num"))
    {
        history_main_funds_num = atoi(_cfg["history_main_funds_num"].c_str());
    }

    if (has_key<string, string>(_cfg, "history_wquotation_num"))
    {
        history_wquotation_num = atoi(_cfg["history_wquotation_num"].c_str());
    }

    if (has_key<string, string>(_cfg, "history_wsingle_num"))
    {
        history_wsingle_num = atoi(_cfg["history_wsingle_num"].c_str());
    }

    if (has_key<string, string>(_cfg, "real_single_deque_length"))
    {
        real_single_deque_length = atoi(_cfg["real_single_deque_length"].c_str());
    }


    if (has_key<string, string>(_cfg, "is_req_single_from_file"))
    {
        is_req_single_from_file = atoi(_cfg["is_req_single_from_file"].c_str());
    }
    if (has_key<string, string>(_cfg, "is_req_main_funds_from_file"))
    {
        is_req_main_funds_from_file = atoi(_cfg["is_req_main_funds_from_file"].c_str());
    }

    if (has_key<string, string>(_cfg, "req_single_from_file_path"))
    {
        req_single_from_file_path = _cfg["req_single_from_file_path"];
    }
    if (has_key<string, string>(_cfg, "req_main_funds_from_file_path"))
    {
        req_main_funds_from_file_path = _cfg["req_main_funds_from_file_path"];
    }


    if (has_key<string, string>(_cfg, "real_quotation_deque_length"))
    {
        real_quotation_deque_length = atoi(_cfg["real_quotation_deque_length"].c_str());
    }

    if (has_key<string, string>(_cfg, "is_req_quotation_from_file"))
    {
        is_req_quotation_from_file = atoi(_cfg["is_req_quotation_from_file"].c_str());
    }

    if (has_key<string, string>(_cfg, "req_quotation_from_file_path"))
    {
        req_quotation_from_file_path = _cfg["req_quotation_from_file_path"];
    }



    if (has_key<string, string>(_cfg, "log_single_deque_length"))
    {
        log_single_deque_length = atoi(_cfg["log_single_deque_length"].c_str());
    }
    if (has_key<string, string>(_cfg, "log_main_funds_deque_length"))
    {
        log_main_funds_deque_length = atoi(_cfg["log_main_funds_deque_length"].c_str());
    }

    if (has_key<string, string>(_cfg, "log_single_deque_length_max"))
    {
        log_single_deque_length_max = atoi(_cfg["log_single_deque_length_max"].c_str());
    }
    if (has_key<string, string>(_cfg, "log_main_funds_deque_length_max"))
    {
        log_main_funds_deque_length_max = atoi(_cfg["log_main_funds_deque_length_max"].c_str());
    }

    if (has_key<string, string>(_cfg, "per_day_min_req_circle_times"))
    {
        per_day_min_req_circle_times = atoi(_cfg["per_day_min_req_circle_times"].c_str());
    }

    if (has_key<string, string>(_cfg, "per_day_perdump_with_circles"))
    {
        per_day_perdump_with_circles = atoi(_cfg["per_day_perdump_with_circles"].c_str());
    }

    if (has_key<string, string>(_cfg, "is_not_trade_date_ratio"))
    {
        is_not_trade_date_ratio = atof(_cfg["is_not_trade_date_ratio"].c_str());
    }

    if (has_key<string, string>(_cfg, "lru_ssr_length"))
    {
        lru_ssr_length = atoi(_cfg["lru_ssr_length"].c_str());
    }

    if (has_key<string, string>(_cfg, "real_quotation_path"))
    {
        real_quotation_path = _cfg["real_quotation_path"];
    }

    if (has_key<string, string>(_cfg, "real_technical_path"))
    {
        real_technical_path = _cfg["real_technical_path"];
    }

    if (has_key<string, string>(_cfg, "real_wtechnical_path"))
    {
        real_wtechnical_path = _cfg["real_wtechnical_path"];
    }

    if (has_key<string, string>(_cfg, "history_wave_path"))
    {
        history_wave_path = _cfg["history_wave_path"];
    }
    if (has_key<string, string>(_cfg, "history_wwave_path"))
    {
        history_wwave_path = _cfg["history_wwave_path"];
    }

    if (has_key<string, string>(_cfg, "real_wquotation_path"))
    {
        real_wquotation_path = _cfg["real_wquotation_path"];
    }

    if (has_key<string, string>(_cfg, "real_wsingle_path"))
    {
        real_wsingle_path = _cfg["real_wsingle_path"];
    }

    if (has_key<string, string>(_cfg, "real_single_path"))
    {
        real_single_path = _cfg["real_single_path"];
    }
    if (has_key<string, string>(_cfg, "real_main_funds_path"))
    {
        real_main_funds_path = _cfg["real_main_funds_path"];
    }

    if (has_key<string, string>(_cfg, "real_quotation_api"))
    {
        real_quotation_api = _cfg["real_quotation_api"];
    }

    if (has_key<string, string>(_cfg, "real_single_api"))
    {
        real_single_api = _cfg["real_single_api"];
    }
    if (has_key<string, string>(_cfg, "real_main_funds_api"))
    {
        real_main_funds_api = _cfg["real_main_funds_api"];
    }

    real_single_scale.clear();
    if (has_key<string, string>(_cfg, "real_single_scale"))
    {
        vector<string> strVec;
        SplitString(_cfg["real_single_scale"].c_str(), ",", &strVec, SPLIT_MODE_ALL);
        for (uint32_t i = 0; i < strVec.size(); i++)
        real_single_scale.push_back(atoi(strVec[i].c_str()));
    }
}

int strategy_conf::reload()
{
    destroy();
    return load();
}

void strategy_conf::set_path (const char* path)
{
    snprintf(_fullpath, sizeof(_fullpath), "%s", path);
}

bool strategy_conf::need_reload()
{
    struct stat st;

    if (stat(_fullpath, &st) == 0 && S_ISREG(st.st_mode) && st.st_mtime != _last_load)
    {
        return true;
    }

    return false;
}

int strategy_conf::dump()
{
    FILE * fp = fopen(_dumppath, "w");
    ASSERT_WARNING(fp != NULL, "finance_dict dump_data failed, open file [%s] error", _dumppath);

    for (auto &ii: _cfg)
    {
        fprintf(fp, "%s:%s\n", ii.first.c_str(), ii.second.c_str());
    }

    fclose(fp);
    return 0;
}

int strategy_conf::destroy()
{
    {
        _cfg.clear();
    }

    {
        string tmp;
        tmp.swap(ua_path);
    }

    {
        string tmp;
        tmp.swap(ua_file);
    }

    {
        string tmp;
        tmp.swap(id_path);
    }


    {
        string tmp;
        tmp.swap(id_file);
    }

    {
        string tmp;
        tmp.swap(financie_path);
    }

    {
        string tmp;
        tmp.swap(financie_file);
    }


    {
        string tmp;
        tmp.swap(ban_path);
    }

    {
        string tmp;
        tmp.swap(ban_file);
    }


    {
        string tmp;
        tmp.swap(history_single_path);
    }

    {
        string tmp;
        tmp.swap(history_main_funds_path);
    }

    {
        string tmp;
        tmp.swap(history_single_file);
    }
    {
        string tmp;
        tmp.swap(history_main_funds_file);
    }

    {
        string tmp;
        tmp.swap(history_quotation_path);
    }

    {
        string tmp;
        tmp.swap(history_quotation_file);
    }

    {
        string tmp;
        tmp.swap(history_technical_path);
    }

    {
        string tmp;
        tmp.swap(history_technical_file);
    }


    {
        string tmp;
        tmp.swap(userid_dict_path);
    }

    {
        string tmp;
        tmp.swap(dump_technical_schema);
    }

    {
        string tmp;
        tmp.swap(dump_wave_schema);
    }
    {
        string tmp;
        tmp.swap(dump_wwave_schema);
    }

    {
        string tmp;
        tmp.swap(dump_single_schema);
    }
    {
        string tmp;
        tmp.swap(dump_main_funds_schema);
    }

    {
        string tmp;
        tmp.swap(dump_quotation_schema);
    }
    {
        string tmp;
        tmp.swap(dump_wsingle_schema);
    }

    {
        string tmp;
        tmp.swap(dump_wquotation_schema);
    }

    {
        string tmp;
        tmp.swap(dump_wtechnical_schema);
    }

    {
        string tmp;
        tmp.swap(userid_dict_file);
    }


    {
        string tmp;
        tmp.swap(history_wsingle_path);
    }


    {
        string tmp;
        tmp.swap(history_wsingle_file);
    }

    {
        string tmp;
        tmp.swap(history_wquotation_path);
    }

    {
        string tmp;
        tmp.swap(history_wquotation_file);
    }


    {
        string tmp;
        tmp.swap(holiday_dict_path);
    }

    {
        string tmp;
        tmp.swap(backup_files_path);
    }

    {
        string tmp;
        tmp.swap(backuped_path);
    }


    {
        string tmp;
        tmp.swap(holiday_dict_file);
    }

    {
        string tmp;
        tmp.swap(plate_dict_split_path);
    }

    {
        string tmp;
        tmp.swap(plate_dict_split_file);
    }


    {
        string tmp;
        tmp.swap(plate_dict_path);
    }

    {
        string tmp;
        tmp.swap(plate_dict_file);
    }

    {
        string tmp;
        tmp.swap(cir_holder_dict_split_path);
    }

    {
        string tmp;
        tmp.swap(cir_holder_dict_split_file);
    }


    {
        string tmp;
        tmp.swap(cir_holder_dict_path);
    }

    {
        string tmp;
        tmp.swap(cir_holder_dict_file);
    }


    {
        string tmp;
        tmp.swap(addr_dict_split_path);
    }

    {
        string tmp;
        tmp.swap(addr_dict_split_file);
    }

    {
        string tmp;
        tmp.swap(addr_dict_path);
    }

    {
        string tmp;
        tmp.swap(addr_dict_file);
    }

    {
        string tmp;
        tmp.swap(real_quotation_path);
    }

    {
        string tmp;
        tmp.swap(history_wave_path);
    }
    {
        string tmp;
        tmp.swap(history_wwave_path);
    }

    {
        string tmp;
        tmp.swap(real_wquotation_path);
    }

    {
        string tmp;
        tmp.swap(real_wsingle_path);
    }

    {
        string tmp;
        tmp.swap(real_technical_path);
    }

    {
        string tmp;
        tmp.swap(real_wtechnical_path);
    }

    {
        string tmp;
        tmp.swap(real_single_path);
    }
    {
        string tmp;
        tmp.swap(real_main_funds_path);
    }

    {
        string tmp;
        tmp.swap(real_quotation_api);
    }

    {
        string tmp;
        tmp.swap(real_single_api);
    }
    {
        string tmp;
        tmp.swap(real_main_funds_api);
    }

    {
        string tmp;
        tmp.swap(real_morning_stime);
    }

    {
        string tmp;
        tmp.swap(backup_stime);
    }

    {
        string tmp;
        tmp.swap(real_morning_etime);
    }

    {
        string tmp;
        tmp.swap(real_afternoon_stime);
    }

    {
        string tmp;
        tmp.swap(real_afternoon_etime);
    }

    {
        string tmp;
        tmp.swap(dump_real_time);
    }

    {
        string tmp;
        tmp.swap(cafile);
    }

    {
        vector<uint32_t> tmp;
        tmp.swap(real_single_scale);
    }


    return 0;
}


