#include "history_quotation_dict.h"
#include "base_def.h"
#include "log_helper.h"
#include "ul_sign.h"
#include "common_util.h"
#include "proc_data.h"
#include "id_dict.h"

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

        std::shared_ptr<quotation_t> qt;
        std::shared_ptr<quotation_original> qo(new quotation_original);

        const std::string &id = strVec[0];
        
        
        auto id_dict = p_data->_id_dict->current();
        auto id_name = id_dict->_id_name_map.find(id);
        if (id_name != id_dict->_id_name_map.end())
        {
            snprintf(qo->name, sizeof(qo->name), "%s", id_name->second.c_str());
        }
        

        qo->start = atof(strVec[1].c_str());
        qo->end = atof(strVec[2].c_str());
        qo->high = atof(strVec[3].c_str());
        qo->low = atof(strVec[4].c_str());
        qo->last_closed = atof(strVec[5].c_str());
        qo->vol = atoi(strVec[6].c_str());
        qo->buy_vol = atoi(strVec[7].c_str());
        qo->sell_vol = atoi(strVec[8].c_str());
        qo->swing = atof(strVec[9].c_str());
        qo->change_rate = atof(strVec[10].c_str());
        qo->range_percent = atof(strVec[11].c_str());
        qo->total_price = atoi(strVec[12].c_str());

        if (strVec.size() > 13)
            qo->quantity_relative_ratio = atoi(strVec[13].c_str());

        qt = original_2_quotation(qo);

        if (date == _last_date)
            _real_dict[id] = qo;

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

void history_quotation_dict::update_real_quotation(const std::string & trade_date, const std::string & id, std::shared_ptr<quotation_original> & qt)
{
    std::string key; 

    creat_key(trade_date, id, key);

    _real_dict[id] = qt;

    _date_dict[key] = original_2_quotation(qt);

    _last_date = trade_date;

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

    update_hquotation_wave(hqitem);
}

void history_quotation_dict::update_hquotation_wave(hquotation_search_item * hqitem)
{
    proc_data* p_data = proc_data::instance();
    if (!hqitem)
        return;

    for (auto ii = hqitem->id_quotation.begin(); ii != hqitem->id_quotation.end(); ii++)
    {
        const std::string & id = ii->first; 
        auto ff = hqitem->id_technical.find(id);
        
        auto kk = _id_date_dict.find(id);
        if (kk == _id_date_dict.end() || ff == hqitem->id_technical.end())
            continue;


        for (int p = 0; p < (int)ii->second.size() && (int)ii->second.size() > 3; p++)
        {   
            if (!p)
            {
                if (ii->second[p]->low <= ii->second[p+1]->low && ii->second[p+1]->low <= ii->second[p+2]->low && ff->second[p]->low_end_5 < 1 &&
                        (ff->second[p]->end_end_5 >= 1 || ff->second[p+1]->end_end_5 >=1 || ff->second[p + 2]->end_end_5 >=1 || 
                         (ii->second.size() > 4 && ff->second[p + 3]->end_end_5 >=1)))
                    hqitem->id_trough[id].insert(p);

                if (ii->second[p]->high >= ii->second[p+1]->high && ii->second[p+1]->high >= ii->second[p+2]->high && ff->second[p]->high_end_5 > 1 &&
                        (ff->second[p]->end_end_5 < 1 || ff->second[p+1]->end_end_5 < 1 || ff->second[p + 2]->end_end_5 < 1 || 
                         (ii->second.size() > 4 && ff->second[p + 3]->end_end_5 < 1)))
                    hqitem->id_crest[id].insert(p);

                continue;
            }

            if (p < (int)ii->second.size() - 2)
            {
                if (ii->second[p+1]->low >= ii->second[p]->low && ii->second[p-1]->low >= ii->second[p]->low && ff->second[p]->low_end_5 < 1 &&
                        (ff->second[p]->end_end_5 >= 1 || ff->second[p+1]->end_end_5 >=1 || ff->second[p + 2]->end_end_5 >=1 || 
                         (ii->second.size() - p > 3 && ff->second[p + 3]->end_end_5 >=1)))
                    hqitem->id_trough[id].insert(p);

                if (ii->second[p]->high >= ii->second[p+1]->high && ii->second[p]->high >= ii->second[p-1]->high && ff->second[p]->high_end_5 > 1 &&
                        (ff->second[p]->end_end_5 < 1 || ff->second[p+1]->end_end_5 < 1 || ff->second[p + 2]->end_end_5 < 1 || 
                         (ii->second.size() -p > 3 && ff->second[p + 3]->end_end_5 < 1)))
                    hqitem->id_crest[id].insert(p);

                continue;
            }
        }
    }
}

void history_quotation_dict::update_rquotation_search()
{
    proc_data* p_data = proc_data::instance();

    auto cur = p_data->_rquotation_index->current();
    auto idl = p_data->_rquotation_index->idle();

    for (auto ii = _real_dict.begin(); ii != _real_dict.end(); ii++)
    {
        const std::string & id = ii->first; 


        {
            std::deque< std::shared_ptr<quotation_original>> dq;
            idl->id_quotation[id] = dq;
            auto mm = cur->id_quotation.find(id);
            if (mm != cur->id_quotation.end()) 
            {
                idl->id_quotation[id] = mm->second;
            }   

            if (idl->id_quotation[id].empty() || idl->id_quotation[id].back() != ii->second)
            {
                idl->id_quotation[id].push_back(ii->second);
            }

            if (idl->id_quotation[id].size() > p_data->_conf->_strategy->current()->real_quotation_deque_length)
            {
                idl->id_quotation[id].pop_front();
            }

            idl->end_index.insert(std::make_pair(ii->second->end, id));
            idl->change_rate_index.insert(std::make_pair(ii->second->change_rate, id));
            idl->range_percent_index.insert(std::make_pair(ii->second->range_percent, id));
        }

        {

            hquotation_search_item * hqitem = p_data->_hquotation_index->idle();

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

                /*
                if (iii->second.back()->end_end_30)
                {
                    idl->end_end30_index.insert(std::make_pair(iii->second.back()->end_end_30, id));
                }
                */
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

    /*
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
    */

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

    /*
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
    */

    if (tt->end_5)
    {
        tt->low_end_5 = qt->low / tt->end_5;
        snprintf(t_buf, sizeof(t_buf), "%.2f", tt->low_end_5); 
        tt->low_end_5 = atof(t_buf);
    }

    if (tt->end_5)
    {
        tt->high_end_5 = qt->high / tt->end_5;
        snprintf(t_buf, sizeof(t_buf), "%.2f", tt->high_end_5); 
        tt->high_end_5 = atof(t_buf);
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

    std::set<std::string> file_set;
    std::set<std::string> date_set;

    while (fgets(line, sizeof(line), fp)) 
    {
        if('\0' == line[0]){
            continue;
        }
        //line[strlen(line) - 1] = '\0';
        ptr = im_chomp(line);
        if (ptr == NULL || *ptr == '\0'|| *ptr == '#')
            continue;

        file_set.insert(std::string(ptr));

        std::string date = basename(ptr);

        date_set.insert(date);

    }

    fclose(fp);

    if (!date_set.empty())
    {
        _last_date = *(date_set.rbegin());
    }

    for (auto filename : file_set)
    {
        load_history_quoation(filename.c_str());
    }

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
        std::unordered_map<std::string, std::shared_ptr<quotation_original>, str_hasher> tmp;
        _real_dict.swap(tmp);
    }
    //
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

std::shared_ptr<quotation_t>  history_quotation_dict::original_2_quotation(std::shared_ptr<quotation_original> & original)
{
    std::shared_ptr<quotation_t> qq = std::make_shared<quotation_t>();

    qq->start = original->start;
    qq->end = original->end; 
    qq->high = original->high; 
    qq->low = original->low; 
    //qq->last_closed = original->last_closed;
    qq->vol = original->vol; 
    //qq->buy_vol = original->buy_vol;
    //qq->sell_vol = original->sell_vol;
    //qq->swing = original->swing;
    qq->change_rate = original->change_rate;
    qq->range_percent = original->range_percent; 
    qq->total_price = original->total_price;

    return qq;
}
