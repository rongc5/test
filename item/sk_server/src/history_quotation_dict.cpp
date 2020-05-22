#include "history_quotation_dict.h"
#include "base_def.h"
#include "log_helper.h"
#include "ul_sign.h"
#include "common_util.h"
#include "proc_data.h"
#include "id_dict.h"

std::shared_ptr<quotation_t> operator + (const std::shared_ptr<quotation_t>  qt, const std::shared_ptr<quotation_t>  mm)
{   
    std::shared_ptr<quotation_t> qq = std::make_shared<quotation_t>();

    qq->start = mm->start + qt->start;
    qq->end = mm->end + qt->end; 
    qq->high = mm->high + qt->high; 
    qq->low = mm->low + qt->low; 
    qq->last_closed = mm->last_closed + qt->last_closed;
    qq->vol = mm->vol + qt->vol; 
    qq->vol = mm->buy_vol + qt->buy_vol;
    qq->sell_vol = mm->sell_vol + qt->sell_vol;
    qq->swing = mm->swing + qt->swing;
    qq->change_rate = mm->change_rate + qt->change_rate;
    qq->range_percent = mm->range_percent + qt->range_percent; 
    qq->total_price = mm->total_price + qt->total_price;

    return qq;
}   

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

    proc_data* p_data = proc_data::instance();
    FILE * fp = fopen(file, "r");
    ASSERT_WARNING(fp != NULL,"open file failed. file[%s]", file);


    std::vector<std::string> strVec;
    char line[SIZE_LEN_1024];
    char t_buf[SIZE_LEN_1024];
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
        const std::string &id = strVec[0];
        
        auto id_dict = p_data->_id_dict->current();
        auto id_name = id_dict->_id_name_map.find(id);
        if (id_name != id_dict->_id_name_map.end())
        {
            snprintf(qt->name, sizeof(qt->name), "%s", id_name->second.c_str());
        }

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

        creat_key(date, id, key);
        auto ii = _date_dict.find(key);
        if (ii == _date_dict.end())
        {
            _date_dict.insert(std::make_pair(key, qt));
        }

        {   
            auto ii = _id_date_dict.find(id);
            if (ii == _id_date_dict.end())
            {   
                std::set<std::string> t_set;
                t_set.insert(date);
                _id_date_dict.insert(std::make_pair(id, t_set));
            }   
            else
            {   
                ii->second.insert(date);
            }   
        }
    }
    fclose(fp);

    return 0;
}

void history_quotation_dict::update_real_quotation(const std::string & trade_date, const std::string & id, std::shared_ptr<quotation_t> & qt)
{
    std::string key; 

    creat_key(trade_date, id, key);

    _date_dict[key] = qt;

    auto ii = _id_date_dict.find(id);
    if (ii == _id_date_dict.end())
    {   
        std::set<std::string> t_set;
        t_set.insert(trade_date);
        _id_date_dict.insert(std::make_pair(id, t_set));
    }   
    else
    {   
        ii->second.insert(trade_date);
    }

}

 void history_quotation_dict::update_search_index()
{
    proc_data* p_data = proc_data::instance();
    p_data->_hquotation_index->idle()->clear();
    p_data->_rquotation_index->idle()->clear();

    update_hquotation_search();
    update_rquotation_search();

    p_data->_hquotation_index->idle_2_current();
    p_data->_rquotation_index->idle_2_current();
}

 void history_quotation_dict::update_hquotation_search()
{
    proc_data* p_data = proc_data::instance();
    auto hqitem = p_data->_hquotation_index->idle();
    for (auto ii = _id_date_dict.begin(); ii != _id_date_dict.end(); ii++)
    {
         const std::string & id = ii->first; 
         int len = 0;
         for (auto iii = ii->second.begin(); iii != ii->second.end(); iii++, len++)
         {
              std::string key;
              creat_key(*iii, id, key);
              auto kk = _date_dict.find(key);
              if (kk == _date_dict.end())
                  continue;

               auto mm = hqitem->id_quotation.find(id);
               if (mm == hqitem->id_quotation.end())
               {
                   std::deque< std::shared_ptr<quotation_t>> tmp_vec;
                   tmp_vec.push_back(kk->second);
                   hqitem->id_quotation.insert(std::make_pair(id, tmp_vec));
               }
              else
              {
                  mm->second.push_back(kk->second);
              }

              std::string item_key;
              hquotation_search_item::creat_id_index_key(id, len, item_key);
              hqitem->id_idx_date.insert(std::make_pair(item_key, *iii));
              hquotation_search_item::creat_id_date_key(id, *iii, item_key);
              hqitem->id_date_idx.insert(std::make_pair(item_key, len));
         }
    }


    for (auto ii = hqitem->id_quotation.begin(); ii != hqitem->id_quotation.end(); ii++)
    {
        const std::string & id = ii->first; 
        
        auto kk = _id_date_dict.find(id);
        if (kk == _id_date_dict.end())
            continue;

        std::shared_ptr<quotation_t> qt = std::make_shared<quotation_t>();
        std::deque<std::shared_ptr<quotation_t>> dp((int)ii->second.size() + 1, qt);


        for (int p = 0; p < (int)ii->second.size(); p++)
        {   
            dp[p+1] = dp[p] + ii->second[p];

            std::shared_ptr<technical_t> tt = std::make_shared<technical_t>();

            get_id_technical(ii->second[p], dp, p, tt);


            auto ff = hqitem->id_technical.find(id);
            if (ff == hqitem->id_technical.end())
            {
                std::deque< std::shared_ptr<technical_t>> dq;
                dq.push_back(tt);
                hqitem->id_technical[id] = dq;
            }
            else
            {
                ff->second.push_back(tt); 
            }
        }

        hqitem->id_sum_quotation[id] = dp;
    }

}

void history_quotation_dict::update_rquotation_search()
{
    proc_data* p_data = proc_data::instance();

    hquotation_search_item * hqitem = p_data->_hquotation_index->idle();
    auto cur = p_data->_rquotation_index->current();
    auto idl = p_data->_rquotation_index->idle();

    for (auto ii = hqitem->id_quotation.begin(); ii != hqitem->id_quotation.end(); ii++)
    {
        const std::string & id = ii->first; 


        if (!ii->second.size())
            continue;

        int index = ii->second.size() - 1;
        std::string date = hqitem->get_date(id, index);
        if (date != p_data->_trade_date)
            continue;

        {
            std::deque< std::shared_ptr<quotation_t>> dq;
            idl->id_quotation[id] = dq;
            auto mm = cur->id_quotation.find(id);
            if (mm != cur->id_quotation.end()) 
            {
                idl->id_quotation[id] = mm->second;
            }   

            if (idl->id_quotation[id].empty() || idl->id_quotation[id].back() != ii->second.back())
            {
                idl->id_quotation[id].push_back(ii->second.back());
            }

            if (idl->id_quotation[id].size() > p_data->_conf->_strategy->current()->real_quotation_deque_length)
            {
                idl->id_quotation[id].pop_front();
            }

            idl->end_index.insert(std::make_pair(ii->second.back()->end, id));
            idl->change_rate_index.insert(std::make_pair(ii->second.back()->change_rate, id));
            idl->range_percent_index.insert(std::make_pair(ii->second.back()->range_percent, id));
        }

        {
            std::deque< std::shared_ptr<technical_t>> dq;
            auto mm = cur->id_technical.find(id);
            if (mm != cur->id_technical.end())
            {
                idl->id_technical[id] = mm->second;
            }
            auto iii = hqitem->id_technical.find(id);

            if (iii != hqitem->id_technical.end())
            {
                idl->id_technical[id].push_back(iii->second.back());

                if (idl->id_technical[id].size() > p_data->_conf->_strategy->current()->real_quotation_deque_length)
                {
                    idl->id_technical[id].pop_front();
                }


                if (iii->second.back()->down_pointer)
                {
                    idl->down_pointer_index.insert(std::make_pair(iii->second.back()->down_pointer, id));
                }
                if (iii->second.back()->up_pointer)
                {
                    idl->up_pointer_index.insert(std::make_pair(iii->second.back()->up_pointer, id));
                }
                if (iii->second.back()->end_avg_end)
                {
                    idl->end_avg_end_index.insert(std::make_pair(iii->second.back()->end_avg_end, id));
                }
                if (iii->second.back()->end_end_5)
                {
                    idl->end_end5_index.insert(std::make_pair(iii->second.back()->end_end_5, id));
                }
                if (iii->second.back()->end_end_10)
                {
                    idl->end_end10_index.insert(std::make_pair(iii->second.back()->end_end_10, id));
                }
                if (iii->second.back()->end_end_20)
                {
                    idl->end_end20_index.insert(std::make_pair(iii->second.back()->end_end_20, id));
                }
                if (iii->second.back()->end_end_30)
                {
                    idl->end_end30_index.insert(std::make_pair(iii->second.back()->end_end_30, id));
                }
            }
        }
    }

}

void history_quotation_dict::get_id_technical(std::shared_ptr<quotation_t> qt, std::deque< std::shared_ptr<quotation_t>> & sum_quotation, int p, std::shared_ptr<technical_t> tt)
{
    char t_buf[SIZE_LEN_1024];
    t_buf[0] = '\0';

    int len = sum_quotation.size();
    if (p >= 5)
    {
        tt->end_5 = (sum_quotation[p]->end - sum_quotation[p - 5]->end)/5;
        snprintf(t_buf, sizeof(t_buf), "%.2f", tt->end_5);
        tt->end_5 = atof(t_buf);
    }

    if (p >= 10)
    {
        tt->end_10 = (sum_quotation[p]->end - sum_quotation[p - 10]->end)/10;
        snprintf(t_buf, sizeof(t_buf), "%.2f", tt->end_10);
        tt->end_10 = atof(t_buf);
    }

    if (p >= 20)
    {
        tt->end_20 = (sum_quotation[p]->end - sum_quotation[p - 20]->end)/20;
        snprintf(t_buf, sizeof(t_buf), "%.2f", tt->end_20);
        tt->end_20 = atof(t_buf);
    }

    if (p >= 30)
    {
        tt->end_30 = (sum_quotation[p]->end - sum_quotation[p - 30]->end)/30;
        snprintf(t_buf, sizeof(t_buf), "%.2f", tt->end_30);
        tt->end_30 = atof(t_buf);
    }

    if (p >= 60)
    {
        tt->end_60 = (sum_quotation[p]->end - sum_quotation[p - 60]->end)/60;
        snprintf(t_buf, sizeof(t_buf), "%.2f", tt->end_60);
        tt->end_60 = atof(t_buf);
    }

    if (tt->end_5)
    {
        tt->end_end_5 = qt->end / tt->end_5;
        snprintf(t_buf, sizeof(t_buf), "%.2f", tt->end_end_5); 
        tt->end_end_5 = atof(t_buf);
    }

    if (tt->end_10)
    {
        tt->end_end_10 = qt->end / tt->end_10;
        snprintf(t_buf, sizeof(t_buf), "%.2f", tt->end_end_10); 
        tt->end_end_10 = atof(t_buf);
    }

    if (tt->end_20)
    {
        tt->end_end_20 = qt->end / tt->end_20;
        snprintf(t_buf, sizeof(t_buf), "%.2f", tt->end_end_20); 
        tt->end_end_20 = atof(t_buf);
    }

    if (tt->end_30)
    {
        tt->end_end_30 = qt->end / tt->end_30;
        snprintf(t_buf, sizeof(t_buf), "%.2f", tt->end_end_30); 
        tt->end_end_30 = atof(t_buf);
    }

    if (tt->end_60)
    {
        tt->end_end_60 = qt->end / tt->end_60;
        snprintf(t_buf, sizeof(t_buf), "%.2f", tt->end_end_60); 
        tt->end_end_60 = atof(t_buf);
    }


    if (qt->end > qt->low)
    {   
        float tmp = qt->end - qt->start;
        if (tmp == 0)
            tmp = 0.01;

        tt->down_pointer = (qt->end - qt->low)/tmp;

        if (tt->down_pointer < 0)
            tt->down_pointer *= -1; 

        snprintf(t_buf, sizeof(t_buf), "%.2f", tt->down_pointer);
        tt->down_pointer = atof(t_buf);
    }

    if (qt->end < qt->high)
    {
        float tmp = qt->end - qt->start;
        if (tmp == 0)
            tmp = 0.01;

        tt->up_pointer = (qt->high - qt->end)/tmp;

        if (tt->up_pointer < 0)
            tt->up_pointer *= -1;

        snprintf(t_buf, sizeof(t_buf), "%.2f", tt->up_pointer);
        tt->up_pointer = atof(t_buf);
    }

    if (qt->start)
    {
        tt->end_start = qt->end / qt->start;
        snprintf(t_buf, sizeof(t_buf), "%.2f", tt->end_start); 
        tt->end_start = atof(t_buf);

    }

    if (qt->vol)
    {
        tt->avg_end = (qt->total_price * 10000)/(qt->vol * 100);
        snprintf(t_buf, sizeof(t_buf), "%.2f", tt->avg_end);
        tt->avg_end = atof(t_buf);
    }

    if (tt->avg_end)
    {
        tt->end_avg_end = qt->end / tt->avg_end;
        snprintf(t_buf, sizeof(t_buf), "%.2f", tt->end_avg_end);
        tt->end_avg_end = atof(t_buf);
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

    update_search_index();

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

    //for (auto &p_data: _date_dict)
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
        _date_dict.swap(tmp);

        //if (tmp.size())
        //{
            //LOG_TRACE("%s, %d", tmp.begin()->first.c_str(), tmp.begin()->second.use_count());
        //}
    }

    {
        std::unordered_map<std::string, std::set<std::string>, str_hasher> tmp;
        _id_date_dict.swap(tmp);
    }


    return 0;
}
