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

int history_quotation_dict::load_history_quoation(const char * file)
{
    if (!file)
    {
        return -1;
    }

    std::string date = basename((char *)file);
    auto ii = _id_dict.find(date);
    if (ii != _id_dict.end())
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

        qt->id = strVec[0];
        qt->start = strVec[1];
        qt->end = strVec[2];
        qt->high = strVec[3];
        qt->low = strVec[4];
        qt->last_closed = strVec[5];
        qt->vol = strVec[6];
        qt->buy_vol = strVec[7];
        qt->sell_vol = strVec[8];
        qt->swing = strVec[9];
        qt->change_rate = strVec[10];
        qt->range_percent = strVec[11];
        qt->total_price = strVec[12];
        qt->down_pointer = strVec[13];
        qt->up_pointer = strVec[14];
        qt->avg_price = strVec[15];


        creat_key(date, qt->id, key);
        auto ii = _id_dict.find(key);
        if (ii == _id_dict.end())
        {
            _id_dict.insert(std::make_pair(key, qt));
        }

        {   
            auto ii = _id_date_dict.find(strVec[0]);
            if (ii == _id_date_dict.end())
            {   
                std::set<std::string> t_set;
                t_set.insert(date);
                _id_date_dict.insert(std::make_pair(strVec[0], t_set));
            }   
            else
            {   
                ii->second.insert(date);
            }   
        }

        {
            auto ii = p_data->_hqend_index.idle()->find(date);
            if (ii == p_data->_hqend_index.idle()->end())
            {
                std::multimap<float, std::string> t_map;

                t_map.insert(std::make_pair(atof(qt->end.c_str()), qt->id));
                p_data->_hqend_index.idle()->insert(std::make_pair(date, t_map));
            }
            else
            {
                ii->second.insert(std::make_pair(atof(qt->end.c_str()), qt->id));
            }
        }

        {
            auto ii = p_data->_hqchange_rate_index.idle()->find(date);
            if (ii == p_data->_hqchange_rate_index.idle()->end())
            {
                std::multimap<float, std::string> t_map;

                t_map.insert(std::make_pair(atof(qt->change_rate.c_str()), qt->id));
                p_data->_hqchange_rate_index.idle()->insert(std::make_pair(date, t_map));
            }
            else
            {
                ii->second.insert(std::make_pair(atof(qt->change_rate.c_str()), qt->id));
            }
        }

        {
            auto ii = p_data->_hqrange_percent_index.idle()->find(date);
            if (ii == p_data->_hqrange_percent_index.idle()->end())
            {
                std::multimap<float, std::string> t_map;

                t_map.insert(std::make_pair(atof(qt->range_percent.c_str()), qt->id));
                p_data->_hqrange_percent_index.idle()->insert(std::make_pair(date, t_map));
            }
            else
            {
                ii->second.insert(std::make_pair(atof(qt->range_percent.c_str()), qt->id));
            }
        }

        if (atof(qt->down_pointer.c_str()))
        {
            auto ii = p_data->_hqdown_pointer_index.idle()->find(date);
            if (ii == p_data->_hqdown_pointer_index.idle()->end())
            {
                std::multimap<float, std::string> t_map;

                t_map.insert(std::make_pair(atof(qt->down_pointer.c_str()), qt->id));
                p_data->_hqdown_pointer_index.idle()->insert(std::make_pair(date, t_map));
            }
            else
            {
                ii->second.insert(std::make_pair(atof(qt->down_pointer.c_str()), qt->id));
            }
        }

        if (atof(qt->up_pointer.c_str()))
        {
            auto ii = p_data->_hqup_pointer_index.idle()->find(date);
            if (ii == p_data->_hqup_pointer_index.idle()->end())
            {
                std::multimap<float, std::string> t_map;

                t_map.insert(std::make_pair(atof(qt->up_pointer.c_str()), qt->id));
                p_data->_hqup_pointer_index.idle()->insert(std::make_pair(date, t_map));
            }
            else
            {

                ii->second.insert(std::make_pair(atof(qt->up_pointer.c_str()), qt->id));
            }
        }
    }

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
            std::shared_ptr<quotation_t> qt(new quotation_t);
            float range_percent = 0;
            float change_rate = 0;

            for (auto iii = it->second.lower_bound(date); iii != it->second.end(); iii++)
            {
                creat_key(*iii, id, key);
                auto tt = _id_dict.find(key);
                if (tt != _id_dict.end())
                {
                    range_percent += atof(tt->second->range_percent.c_str());
                    change_rate += atof(tt->second->change_rate.c_str());
                }
            }

            {
                snprintf(t_buf, sizeof(t_buf), "%.2f", range_percent);
                qt->range_percent.append(t_buf);

                snprintf(t_buf, sizeof(t_buf), "%.2f", change_rate);
                qt->change_rate.append(t_buf);

                creat_key(date, id, key);
                _id_sum_dict.insert(std::make_pair(key, qt));
            }

            {   
                std::map<std::string, std::multimap<float, std::string> >  & u_map = *(p_data->_hq_sum_range_percent_index.idle());   
                auto ii = u_map.find(date);
                if (ii == u_map.end())
                {   
                    std::multimap<float, std::string> t_map;

                    t_map.insert(std::make_pair(range_percent, id));
                    u_map.insert(std::make_pair(date, t_map));
                }       
                else
                {   
                    ii->second.insert(std::make_pair(range_percent, id));
                }
            }

            {
                std::map<std::string, std::multimap<float, std::string> >  & u_map = *(p_data->_hq_sum_change_rate_index.idle());

                auto ii = u_map.find(date);
                if (ii == u_map.end())
                {
                    std::multimap<float, std::string> t_map;

                    t_map.insert(std::make_pair(change_rate, id));
                    u_map.insert(std::make_pair(date, t_map));
                }
                else
                {
                    ii->second.insert(std::make_pair(change_rate, id));
                }
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
        p_data->_hqend_index.idle_2_current();
        p_data->_hqchange_rate_index.idle_2_current();
        p_data->_hqrange_percent_index.idle_2_current();
        p_data->_hqdown_pointer_index.idle_2_current();
        p_data->_hqup_pointer_index.idle_2_current();
        p_data->_hq_sum_range_percent_index.idle_2_current();
        p_data->_hq_sum_change_rate_index.idle_2_current();
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

#if 0
int history_quotation_dict::destroy()
{
    proc_data* p_data = proc_data::instance();

    {
        std::unordered_map<std::string, std::shared_ptr<quotation_t>, str_hasher> tmp;
        _id_dict.swap(tmp);
    }

    {
        std::unordered_map<std::string, std::shared_ptr<quotation_t>, str_hasher> tmp;
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
        std::map<std::string, std::multimap<float, std::string> > t_map;
        p_data->_hq_sum_range_percent_index.idle()->swap(t_map);
    }

    {
        std::map<std::string, std::multimap<float, std::string> > t_map;
        p_data->_hq_sum_change_rate_index.idle()->swap(t_map);
    }

    {
        proc_data* p_data = proc_data::instance();
        if (p_data)
        {
            {
                std::map<std::string, std::multimap<float, std::string> > tmp;
                p_data->_hqend_index.idle()->swap(tmp);
            }

            {
                std::map<std::string, std::multimap<float, std::string> > tmp;
                p_data->_hqchange_rate_index.idle()->swap(tmp);
            }

            {
                std::map<std::string, std::multimap<float, std::string> > tmp;
                p_data->_hqrange_percent_index.idle()->swap(tmp);
            }
            
            {
                std::map<std::string, std::multimap<float, std::string> > tmp;
                p_data->_hqdown_pointer_index.idle()->swap(tmp);
            }

            {
                std::map<std::string, std::multimap<float, std::string> > tmp;
                p_data->_hqup_pointer_index.idle()->swap(tmp);
            }
        }
    }

    return 0;
}

#endif

int history_quotation_dict::destroy()
{
    proc_data* p_data = proc_data::instance();

    {
        std::unordered_map<std::string, std::shared_ptr<quotation_t>, str_hasher> tmp;
        _id_dict.clear();
    }

    {
        std::unordered_map<std::string, std::shared_ptr<quotation_t>, str_hasher> tmp;
        _id_sum_dict.clear();
    }

    {
        std::unordered_map<std::string, std::set<std::string>, str_hasher> tmp;
        _id_date_dict.clear();
    }

    {
        std::set<std::string> tmp;
        _date_index.clear();
    }

    {
        std::map<std::string, std::multimap<float, std::string> > t_map;
        p_data->_hq_sum_range_percent_index.idle()->clear();
    }

    {
        std::map<std::string, std::multimap<float, std::string> > t_map;
        p_data->_hq_sum_change_rate_index.idle()->clear();
    }

    {
        proc_data* p_data = proc_data::instance();
        if (p_data)
        {
            {
                std::map<std::string, std::multimap<float, std::string> > tmp;
                p_data->_hqend_index.idle()->clear();
            }

            {
                std::map<std::string, std::multimap<float, std::string> > tmp;
                p_data->_hqchange_rate_index.idle()->clear();
            }

            {
                std::map<std::string, std::multimap<float, std::string> > tmp;
                p_data->_hqrange_percent_index.idle()->clear();
            }
            
            {
                std::map<std::string, std::multimap<float, std::string> > tmp;
                p_data->_hqdown_pointer_index.idle()->clear();
            }

            {
                std::map<std::string, std::multimap<float, std::string> > tmp;
                p_data->_hqup_pointer_index.idle()->clear();
            }
        }
    }

    return 0;
}

