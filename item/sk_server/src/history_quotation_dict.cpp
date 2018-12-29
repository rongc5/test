#include "history_quotation_dict.h"
#include "base_def.h"
#include "log_helper.h"
#include "ul_sign.h"
#include "common_util.h"
#include "proc_data.h"

history_quotation_dict::history_quotation_dict()
{
}

history_quotation_dict::~history_quotation_dict()
{
    destroy();
}

int history_quotation_dict::init(const char * path, const char * file, const char *dump_dir)
{
    snprintf(_fullpath, sizeof(_fullpath), "%s/%s", path, file);
    snprintf(_dumppath, sizeof(_dumppath), "%s/%s", dump_dir, file);

    destroy();

    return 0;
}

void history_quotation_dict::creat_key(const std::string & date, const std::string & id, std::string & key)
{
    key.clear();

    key.append(date);
    key.append("_");
    key.append(id);

    return;
}

void history_quotation_dict::get_last_date(int n, std::string & date)
{
    if (n <= 0)
        return;

    int k = 0;
    for (auto ii = _date_index.rbegin(); ii != _date_index.rend() && k < n; ii++, k++)
    {
        date = *ii;
    }

    if (k != n)
    {
        date.clear();
    }
}

int history_quotation_dict::load_history_quoation(const char * file)
{
    if (!file)
    {
        return -1;
    }

    std::string date = basename((char *)file);
    auto ii = _date_index.find(date);
    if (ii != _date_index.end())
    {
        LOG_WARNING("file:%s, date:%s has loaded", file, date.c_str());
        return -1;
    }
    _date_index.insert(date);

    proc_data* p_data = proc_data::instance();
    FILE * fp = fopen(file, "r");
    ASSERT_WARNING(fp != NULL,"open file failed. file[%s]", file);


    std::vector<std::string> strVec;
    char line[SIZE_LEN_1024];
    char * ptr = NULL;
    std::string key;
    while (fgets(line, sizeof(line), fp))
    {
        if('\0' == line[0])
        {
            continue;
        }

        ptr = im_chomp(line); 
        if (ptr == NULL || *ptr == '\0'|| *ptr == '#')
            continue;
        
        SplitString(ptr, '\t', &strVec, SPLIT_MODE_ALL | SPLIT_MODE_TRIM);
        
        if (strVec.size() < 13)
        {
            LOG_WARNING("file:%s, format err", file);
            continue;
        }

        std::shared_ptr<quotation_t> qt(new quotation_t);

        snprintf(qt->id, sizeof(qt->id), "%s", strVec[0].c_str());
        qt->start = atof(strVec[1].c_str());
        qt->end = atof(strVec[2].c_str());
        qt->high = atof(strVec[3].c_str());
        qt->low = atof(strVec[4].c_str());
        qt->last_closed = atof(strVec[5].c_str());
        qt->vol = atoi(strVec[6].c_str());
        qt->buy_vol = atoi(strVec[7].c_str());
        qt->sell_vol = atoi(strVec[8].c_str());
        qt->swing = atof(strVec[9].c_str());
        qt->change_rate = atof(strVec[10].c_str());
        qt->range_percent = atof(strVec[11].c_str());
        qt->total_price = atoi(strVec[12].c_str());

        creat_key(date, qt->id, key);
        auto ii = _id_dict.find(key);
        if (ii == _id_dict.end())
        {
            _id_dict.insert(std::make_pair(key, qt));
        }

        {   
            auto ii = _id_date_dict.find(qt->id);
            if (ii == _id_date_dict.end())
            {   
                std::set<std::string> t_set;
                t_set.insert(date);
                _id_date_dict.insert(std::make_pair(qt->id, t_set));
            }   
            else
            {   
                ii->second.insert(date);
            }   
        }

        if (qt->start)
        {
            auto ii = p_data->_hqend_hqstart_index->idle()->find(date);
            if (ii == p_data->_hqend_hqstart_index->idle()->end())
            {
                std::multimap<float, std::string> t_map;

                t_map.insert(std::make_pair(qt->end/qt->start, qt->id));
                p_data->_hqend_hqstart_index->idle()->insert(std::make_pair(date, t_map));
            }
            else
            {
                ii->second.insert(std::make_pair(qt->end/qt->start, qt->id));
            }
        }

        {
            auto ii = p_data->_hqchange_rate_index->idle()->find(date);
            if (ii == p_data->_hqchange_rate_index->idle()->end())
            {
                std::multimap<float, std::string> t_map;

                t_map.insert(std::make_pair(qt->change_rate, qt->id));
                p_data->_hqchange_rate_index->idle()->insert(std::make_pair(date, t_map));
            }
            else
            {
                ii->second.insert(std::make_pair(qt->change_rate, qt->id));
            }
        }

        {
            auto ii = p_data->_hqrange_percent_index->idle()->find(date);
            if (ii == p_data->_hqrange_percent_index->idle()->end())
            {
                std::multimap<float, std::string> t_map;

                t_map.insert(std::make_pair(qt->range_percent, qt->id));
                p_data->_hqrange_percent_index->idle()->insert(std::make_pair(date, t_map));
            }
            else
            {
                ii->second.insert(std::make_pair(qt->range_percent, qt->id));
            }
        }
    }
    fclose(fp);

    return 0;
}

void history_quotation_dict::update_sum_index()
{
    char t_buf[SIZE_LEN_512];
    proc_data* p_data = proc_data::instance();
    if (!p_data)
    {
        return;
    }

    std::string key;
    for (auto ii = _date_index.begin(); ii != _date_index.end(); ii++)
    {
        const std::string  & date = *ii;

        for (auto it = _id_date_dict.begin(); it != _id_date_dict.end(); it++)
        {
            const std::string & id = it->first;
            std::shared_ptr<sum_quotation> qt(new sum_quotation);

            for (auto iii = it->second.lower_bound(date); iii != it->second.end(); iii++)
            {
                creat_key(*iii, id, key);
                auto tt = _id_dict.find(key);
                if (tt != _id_dict.end())
                {
                    qt->range_percent += tt->second->range_percent;
                    qt->change_rate += tt->second->change_rate;

                    qt->avg_end += (tt->second->total_price * 10000)/(tt->second->vol * 100);
                    qt->end += tt->second->end;
                }
            }

            {
                snprintf(t_buf, sizeof(t_buf), "%.2f", qt->range_percent);
                qt->range_percent = atof(t_buf);

                snprintf(t_buf, sizeof(t_buf), "%.2f", qt->change_rate);
                qt->change_rate = atof(t_buf);

                snprintf(t_buf, sizeof(t_buf), "%.2f", qt->avg_end);
                qt->avg_end = atof(t_buf);

                snprintf(t_buf, sizeof(t_buf), "%.2f", qt->end);
                qt->end = atof(t_buf);

                creat_key(date, id, key);
                _id_sum_dict.insert(std::make_pair(key, qt));
            }
        }
    }
}

int history_quotation_dict::load()
{
    FILE * fp = fopen(_fullpath, "r");
    ASSERT_WARNING(fp != NULL,"open query dict failed. path[%s]", _fullpath);

    char line[SIZE_LEN_1024];
    char * ptr = NULL, *bname;
    uint32_t query_sign[2];

    while (fgets(line, sizeof(line), fp)) 
    {
        if('\0' == line[0]){
            continue;
        }
        //line[strlen(line) - 1] = '\0';
        ptr = im_chomp(line);
        if (ptr == NULL || *ptr == '\0'|| *ptr == '#')
            continue;

        load_history_quoation(ptr);
    }

    fclose(fp);
    struct stat st;
    stat(_fullpath, &st);
    _last_load = st.st_mtime;

    update_sum_index();

    proc_data* p_data = proc_data::instance();
    if (p_data)
    {
        p_data->_hqend_hqstart_index->idle_2_current();
        p_data->_hqchange_rate_index->idle_2_current();
        p_data->_hqrange_percent_index->idle_2_current();
    }

    return 0;
}

int history_quotation_dict::reload()
{
    destroy();

    return load();
}

void history_quotation_dict::set_path (const char* path)
{
    snprintf(_fullpath, sizeof(_fullpath), "%s", path);
}

bool history_quotation_dict::need_reload()
{
    struct stat st;

    if (stat(_fullpath, &st) == 0 && S_ISREG(st.st_mode) && st.st_mtime != _last_load)
    {
        return true;
    }

    return false;
}

int history_quotation_dict::dump()
{
    //FILE * fp = fopen(_dumppath, "w");
    //ASSERT_WARNING(fp != NULL, "history_quotation_dict dump_data failed, open file [%s] error", _dumppath);

    //for (auto &p_data: _id_dict)
    //{
        //fprintf(fp,"id[%s]\tpe[%.2f]\tpb[%.2f]\t",p_data.second.id.c_str(), p_data.second.pe, p_data.second.pb);

        //fprintf(fp, "cir_value[%.2f]\tvalue[%.2f]\tmgsy[%.2f]\tmgxj[%.2f]\tmgsygr[%.2f]\t", 
                //p_data.second.cir_value, 
                //p_data.second.value, p_data.second.mgsy, p_data.second.mgxj, p_data.second.mgsygr);

        //fprintf(fp, "mgxjgr[%.2f]\tzysrgr[%.2f]\tyylrgr[%.2f]\tjlrgr[%.2f]time_str[%s]\n", 
                //p_data.second.mgxjgr, p_data.second.zysrgr, p_data.second.yylrgr, p_data.second.jlrgr, 
                //p_data.second.time_str.c_str());
    //}
    //fclose(fp);

    return 0;
}

int history_quotation_dict::destroy()
{
    proc_data* p_data = proc_data::instance();

    //LOG_TRACE("%p", this);
    {
        std::unordered_map<std::string, std::shared_ptr<quotation_t>, str_hasher> tmp;
        _id_dict.swap(tmp);

        if (tmp.size())
        {
            LOG_TRACE("%s, %d", tmp.begin()->first.c_str(), tmp.begin()->second.use_count());
        }
    }

    {
        std::unordered_map<std::string, std::shared_ptr<sum_quotation>, str_hasher> tmp;
        _id_sum_dict.swap(tmp);
    }

    {
        std::unordered_map<std::string, std::set<std::string>, str_hasher> tmp;
        _id_date_dict.swap(tmp);
    }

    {
        std::set<std::string> tmp;
        _date_index.swap(tmp);
    }

    {
        proc_data* p_data = proc_data::instance();
        if (p_data)
        {
            {
                std::map<std::string, std::multimap<float, std::string> > tmp;
                p_data->_hqend_hqstart_index->idle()->swap(tmp);
            }

            {
                std::map<std::string, std::multimap<float, std::string> > tmp;
                p_data->_hqchange_rate_index->idle()->swap(tmp);
            }

            {
                std::map<std::string, std::multimap<float, std::string> > tmp;
                p_data->_hqrange_percent_index->idle()->swap(tmp);
            }
        }
    }

    return 0;
}
