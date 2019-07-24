#include "strategy_conf.h"
#include "base_def.h"
#include "log_helper.h"
#include "common_util.h"

strategy_conf::strategy_conf()
{
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
    std::vector<std::string> tmp_vec;
    while (fgets(line, 1024, fp))
    {
        if('\0' == line[0]){
            continue;
        }

        ptr = im_chomp(line);
        if (ptr == NULL || *ptr == '\0'|| *ptr == '#')
            continue;
        
        SplitString(ptr, "=", &tmp_vec, SPLIT_MODE_ONE | SPLIT_MODE_TRIM);
        if (tmp_vec.size() != 2)
            continue;

        _cfg.insert(std::make_pair(trim(tmp_vec[0].c_str()), trim(tmp_vec[1].c_str())));
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
    if (has_key<std::string, std::string>(_cfg, "real_morning_stime"))
    {
        real_morning_stime = _cfg["real_morning_stime"];
    }

    if (has_key<std::string, std::string>(_cfg, "real_morning_etime"))
    {
        real_morning_etime = _cfg["real_morning_etime"];
    }

    if (has_key<std::string, std::string>(_cfg, "real_afternoon_stime"))
    {
        real_afternoon_stime = _cfg["real_afternoon_stime"];
    }

    if (has_key<std::string, std::string>(_cfg, "real_afternoon_etime"))
    {
        real_afternoon_etime = _cfg["real_afternoon_etime"];
    }

    if (has_key<std::string, std::string>(_cfg, "dump_real_time"))
    {
        dump_real_time = _cfg["dump_real_time"];
    }

    if (has_key<std::string, std::string>(_cfg, "req_http_timeout"))
    {
        req_http_timeout = atoi(_cfg["req_http_timeout"].c_str());
    }

    if (has_key<std::string, std::string>(_cfg, "req_quotation_millisecond"))
    {
        req_quotation_millisecond = atoi(_cfg["req_quotation_millisecond"].c_str());
    }

    if (has_key<std::string, std::string>(_cfg, "destroy_idle_millisecond"))
    {
        destroy_idle_millisecond = atoi(_cfg["destroy_idle_millisecond"].c_str());
    }

    if (has_key<std::string, std::string>(_cfg, "req_single_millisecond"))
    {
        req_single_millisecond = atoi(_cfg["req_single_millisecond"].c_str());
    }

    if (has_key<std::string, std::string>(_cfg, "reload_millisecond"))
    {
        reload_millisecond = atoi(_cfg["reload_millisecond"].c_str());
    }

    if (has_key<std::string, std::string>(_cfg, "ua_path"))
    {
        ua_path = _cfg["ua_path"];
    }

    if (has_key<std::string, std::string>(_cfg, "ua_file"))
    {
        ua_file = _cfg["ua_file"];
    }

    if (has_key<std::string, std::string>(_cfg, "ua_path"))
    {
        ua_path = _cfg["ua_path"];
    }

    if (has_key<std::string, std::string>(_cfg, "id_path"))
    {
        id_path = _cfg["id_path"];
    }

    if (has_key<std::string, std::string>(_cfg, "id_file"))
    {
        id_file = _cfg["id_file"];
    }

    if (has_key<std::string, std::string>(_cfg, "financie_path"))
    {
        financie_path = _cfg["financie_path"];
    }

    if (has_key<std::string, std::string>(_cfg, "financie_file"))
    {
        financie_file = _cfg["financie_file"];
    }

    if (has_key<std::string, std::string>(_cfg, "ban_path"))
    {
        ban_path = _cfg["ban_path"];
    }

    if (has_key<std::string, std::string>(_cfg, "ban_file"))
    {
        ban_file = _cfg["ban_file"];
    }

    if (has_key<std::string, std::string>(_cfg, "plate_dict_split_path"))
    {
        plate_dict_split_path = _cfg["plate_dict_split_path"];
    }

    if (has_key<std::string, std::string>(_cfg, "plate_dict_split_file"))
    {
        plate_dict_split_file = _cfg["plate_dict_split_file"];
    }

    if (has_key<std::string, std::string>(_cfg, "plate_dict_path"))
    {
        plate_dict_path = _cfg["plate_dict_path"];
    }

    if (has_key<std::string, std::string>(_cfg, "plate_dict_file"))
    {
        plate_dict_file = _cfg["plate_dict_file"];
    }

    if (has_key<std::string, std::string>(_cfg, "addr_dict_split_path"))
    {
        addr_dict_split_path = _cfg["addr_dict_split_path"];
    }

    if (has_key<std::string, std::string>(_cfg, "addr_dict_split_file"))
    {
        addr_dict_split_file = _cfg["addr_dict_split_file"];
    }

    if (has_key<std::string, std::string>(_cfg, "addr_dict_path"))
    {
        addr_dict_path = _cfg["addr_dict_path"];
    }

    if (has_key<std::string, std::string>(_cfg, "addr_dict_file"))
    {
        addr_dict_file = _cfg["addr_dict_file"];
    }

    if (has_key<std::string, std::string>(_cfg, "history_single_path"))
    {
        history_single_path = _cfg["history_single_path"];
    }

    if (has_key<std::string, std::string>(_cfg, "history_single_file"))
    {
        history_single_file = _cfg["history_single_file"];
    }

    if (has_key<std::string, std::string>(_cfg, "history_quotation_path"))
    {
        history_quotation_path = _cfg["history_quotation_path"];
    }

    if (has_key<std::string, std::string>(_cfg, "history_quotation_file"))
    {
        history_quotation_file = _cfg["history_quotation_file"];
    }

    if (has_key<std::string, std::string>(_cfg, "holiday_dict_path"))
    {
        holiday_dict_path = _cfg["holiday_dict_path"];
    }

    if (has_key<std::string, std::string>(_cfg, "holiday_dict_file"))
    {
        holiday_dict_file = _cfg["holiday_dict_file"];
    }

    if (has_key<std::string, std::string>(_cfg, "max_reqhttp_num"))
    {
        max_reqhttp_num = atoi(_cfg["max_reqhttp_num"].c_str());
    }

    if (has_key<std::string, std::string>(_cfg, "history_quotation_num"))
    {
        history_quotation_num = atoi(_cfg["history_quotation_num"].c_str());
    }

    if (has_key<std::string, std::string>(_cfg, "history_single_num"))
    {
        history_single_num = atoi(_cfg["history_single_num"].c_str());
    }

    if (has_key<std::string, std::string>(_cfg, "real_single_deque_length"))
    {
        real_single_deque_length = atoi(_cfg["real_single_deque_length"].c_str());
    }

    if (has_key<std::string, std::string>(_cfg, "real_quotation_deque_length"))
    {
        real_quotation_deque_length = atoi(_cfg["real_quotation_deque_length"].c_str());
    }

    if (has_key<std::string, std::string>(_cfg, "real_quotation_path"))
    {
        real_quotation_path = _cfg["real_quotation_path"];
    }

    if (has_key<std::string, std::string>(_cfg, "real_single_path"))
    {
        real_single_path = _cfg["real_single_path"];
    }

    if (has_key<std::string, std::string>(_cfg, "real_quotation_api"))
    {
        real_quotation_api = _cfg["real_quotation_api"];
    }

    if (has_key<std::string, std::string>(_cfg, "real_single_api"))
    {
        real_single_api = _cfg["real_single_api"];
    }

    real_single_scale.clear();
    if (has_key<std::string, std::string>(_cfg, "real_single_scale"))
    {
        std::vector<std::string> strVec;
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
        std::string tmp;
        tmp.swap(ua_path);
    }

    {
        std::string tmp;
        tmp.swap(ua_file);
    }

    {
        std::string tmp;
        tmp.swap(id_path);
    }


    {
        std::string tmp;
        tmp.swap(id_file);
    }

    {
        std::string tmp;
        tmp.swap(financie_path);
    }

    {
        std::string tmp;
        tmp.swap(financie_file);
    }


    {
        std::string tmp;
        tmp.swap(ban_path);
    }

    {
        std::string tmp;
        tmp.swap(ban_file);
    }


    {
        std::string tmp;
        tmp.swap(history_single_path);
    }


    {
        std::string tmp;
        tmp.swap(history_single_file);
    }

    {
        std::string tmp;
        tmp.swap(history_quotation_path);
    }

    {
        std::string tmp;
        tmp.swap(history_quotation_file);
    }

    {
        std::string tmp;
        tmp.swap(holiday_dict_path);
    }


    {
        std::string tmp;
        tmp.swap(holiday_dict_file);
    }

    {
        std::string tmp;
        tmp.swap(plate_dict_split_path);
    }

    {
        std::string tmp;
        tmp.swap(plate_dict_split_file);
    }


    {
        std::string tmp;
        tmp.swap(plate_dict_path);
    }

    {
        std::string tmp;
        tmp.swap(plate_dict_file);
    }

    {
        std::string tmp;
        tmp.swap(addr_dict_split_path);
    }

    {
        std::string tmp;
        tmp.swap(addr_dict_split_file);
    }

    {
        std::string tmp;
        tmp.swap(addr_dict_path);
    }

    {
        std::string tmp;
        tmp.swap(addr_dict_file);
    }

    {
        std::string tmp;
        tmp.swap(real_quotation_path);
    }

    {
        std::string tmp;
        tmp.swap(real_single_path);
    }

    {
        std::string tmp;
        tmp.swap(real_quotation_api);
    }

    {
        std::string tmp;
        tmp.swap(real_single_api);
    }

    {
        std::string tmp;
        tmp.swap(real_morning_stime);
    }

    {
        std::string tmp;
        tmp.swap(real_morning_etime);
    }

    {
        std::string tmp;
        tmp.swap(real_afternoon_stime);
    }

    {
        std::string tmp;
        tmp.swap(real_afternoon_etime);
    }

    {
        std::string tmp;
        tmp.swap(dump_real_time);
    }

    {
        std::vector<uint32_t> tmp;
        tmp.swap(real_single_scale);
    }


    return 0;
}


