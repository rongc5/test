#include "history_wsingle_dict.h"
#include "base_def.h"
#include "log_helper.h"
#include "ul_sign.h"
#include "common_util.h"
#include "proc_data.h"
#include "hsingle_search_index.h"
#include "holiday_dict.h"
#include "sk_util.h"

history_wsingle_dict::history_wsingle_dict()
{
}

history_wsingle_dict::~history_wsingle_dict()
{
    destroy();
}

int history_wsingle_dict::init(const char * path, const char * file, const char *dump_dir)
{
    snprintf(_fullpath, sizeof(_fullpath), "%s/%s", path, file);
    snprintf(_dumppath, sizeof(_dumppath), "%s/%s", dump_dir, file);

    destroy();

    return 0;
}

int history_wsingle_dict::load_history_wsingle(const std::string & week, const char * file)
{
    if (!file)
    {
        return -1;
    }

    const std::string & date = week;

    proc_data* p_data = proc_data::instance();
    strategy_conf * strategy = p_data->_conf->_strategy->current();
    uint32_t log_single_deque_length = p_data->_conf->_strategy->current()->log_single_deque_length;

    FILE * fp = fopen(file, "r");
    ASSERT_WARNING(fp != NULL,"open file failed. file[%s]", file);


    std::vector<std::string> strVec;
    std::vector<std::string> schemaVec;
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

        if (!strstr(ptr, ";"))
            SplitString(ptr, '\t', &strVec, SPLIT_MODE_ALL | SPLIT_MODE_TRIM); 
        else
        {   
            if (strstr(ptr, "id")) 
            {   
                SplitString(ptr, ';', &schemaVec, SPLIT_MODE_ALL | SPLIT_MODE_TRIM);
                continue;
            } else
                SplitString(ptr, ';', &strVec, SPLIT_MODE_ALL | SPLIT_MODE_TRIM);
        }


        std::deque<std::shared_ptr<single_vec> > st;
        std::string & id = strVec[0];

        if (strstr(ptr, ";"))
        {
            std::vector<std::string> tmpVec;
            for (uint32_t i = 1; i < strVec.size(); i ++)
            {

                SplitString(strVec[i].c_str(), ',', &tmpVec, SPLIT_MODE_ALL | SPLIT_MODE_TRIM); 

                uint32_t j = 0;
                if (tmpVec.size() > log_single_deque_length && log_single_deque_length > 0)
                {
                    j = tmpVec.size() - log_single_deque_length;
                }

                while (st.size() < tmpVec.size() - j)
                {
                    std::shared_ptr<single_vec> single(new single_vec);
                    st.push_back(single);
                }

                uint32_t k = 0;
                for (; j < tmpVec.size(); j++, k++)
                {
                    single_t hs;
                    if (strstr(tmpVec[j].c_str(), "|"))
                    {
                        std::vector<std::string> ttVec;
                        SplitString(tmpVec[j].c_str(), '|', &ttVec, SPLIT_MODE_ALL | SPLIT_MODE_TRIM);
                        if (ttVec.size() < 2)
                        {
                            continue;
                        }

                        hs.in = atoi(ttVec[0].c_str());
                        hs.out = atoi(ttVec[1].c_str());
                        hs.diff = hs.in - hs.out;
                    }
                    else
                    {
                        hs.diff = atoi(tmpVec[j].c_str());
                    }
                    hs.dratio = (hs.in + hs.out)? hs.diff*1.0/(hs.in + hs.out):0;

                    st[k]->push_back(hs);
                }
            }  
        }
        else
        {
            std::vector<std::string> tmpVec;
            for (uint32_t i = 1; i < strVec.size(); i ++)
            {
                SplitString(strVec[i].c_str(), ',', &tmpVec, SPLIT_MODE_ALL | SPLIT_MODE_TRIM); 
                
                uint32_t j = 0;
                if (tmpVec.size() > log_single_deque_length && log_single_deque_length > 0)
                {
                    j = tmpVec.size() - log_single_deque_length;
                }

                while (st.size() < tmpVec.size() - j)
                {
                    std::shared_ptr<single_vec> single(new single_vec);
                    st.push_back(single);
                }

                uint32_t k = 0;
                for (; j < tmpVec.size(); j++, k++)
                {

                    single_t hs; 
                    if (strstr(tmpVec[j].c_str(), ":"))
                    {   
                        std::vector<std::string> ttVec;
                        SplitString(tmpVec[j].c_str(), ':', &ttVec, SPLIT_MODE_ALL | SPLIT_MODE_TRIM);
                        if (ttVec.size() < 2)
                        {   
                            continue;
                        }   

                        hs.in = atoi(ttVec[0].c_str());
                        hs.out = atoi(ttVec[1].c_str());
                        hs.diff = hs.in - hs.out;
                    }   
                    else
                    {   
                        hs.diff = atoi(tmpVec[j].c_str());
                    }   

                    st[k]->push_back(hs);
                }
            }
        }
        
        if (st.empty())
            continue;

        complement(st, log_single_deque_length);

        update_date_dict(date, id, st);

        update_id_date(date, id);
    }
    fclose(fp);

    return 0;
}

void history_wsingle_dict::complement(std::deque<std::shared_ptr<single_vec> > & st, int num)
{
    //补齐， 都一样
    if (st.empty())
        return;
    std::shared_ptr<single_vec> sst = st.back();
    while ((int)st.size() < num)
    {
        auto xp = std::make_shared<single_vec>(*sst);
        st.push_back(xp);
    }
}

void history_wsingle_dict::update()
{
    char t_buf[SIZE_LEN_512];
    char path_buf[SIZE_LEN_512];
    std::set<std::string> files;

    proc_data* p_data = proc_data::instance();
    if (!p_data)
        return ;

    strategy_conf * strategy = p_data->_conf->_strategy->current();

    struct dirent *r;
    DIR *p;
    uint32_t i = 0;

    p = opendir(strategy->real_wsingle_path.c_str());
    if (p == NULL)
    {   
        LOG_WARNING("opendir:%s", strategy->real_wsingle_path.c_str());
        return;
    } 

    while ((r= readdir(p)) != NULL)
    {   
        if (r->d_type == DT_REG && start_with(r->d_name, "20"))
        {
            snprintf(t_buf, sizeof(t_buf), "%s/%s", strategy->real_wsingle_path.c_str(), r->d_name);
            files.insert(t_buf);
        }
    } 
    closedir(p);

    snprintf(t_buf, sizeof(t_buf), "%s/.tmp_single", strategy->history_wsingle_path.c_str());
    FILE * fp = fopen(t_buf, "a");
    if (!fp){ 
        return;
    }

    i = 0;
    for (auto ii = files.rbegin(); ii != files.rend() && i < strategy->history_wsingle_num; ii++, i++)
    {
        fprintf(fp, "%s\n", ii->c_str());
    } 

    fclose(fp);
    snprintf(path_buf, sizeof(path_buf), "%s/%s", strategy->history_wsingle_path.c_str(), strategy->history_wsingle_file.c_str());
    int ret = rename(t_buf, path_buf);
    if (ret < 0)
    {
        LOG_WARNING("rename t_buf:%s to path_buf:%s, err:%s", t_buf, path_buf, strerror(errno));
    }
}

void history_wsingle_dict::update_date_dict(const std::string & date, const std::string & id, const std::deque<std::shared_ptr<single_vec>> & st)
{
    std::string key;

    creat_key(date, id, key);
    {
        auto ii = _date_dict.find(key);
        if (ii == _date_dict.end())
        {
            _date_dict.insert(std::make_pair(key, st));
        }
        else
        {
            for (int p = 0; p < (int)ii->second.size(); p++) 
            {
                for (int k =0; k < (int)ii->second.back()->size(); k++)
                {
                    ii->second[p]->at(k).diff += st[p]->at(k).diff;
                }
            }
        }
    }
}

void history_wsingle_dict::update_real_wsingle(const std::string & trade_date, const std::string & id, std::shared_ptr<single_vec>  single)
{
    _last_date_dict[id] = single;
    _last_date = trade_date;
}

void history_wsingle_dict::update_search_index()
{
    proc_data* p_data = proc_data::instance();
    holiday_dict * _holiday_dict = p_data->_holiday_dict->current();

    strategy_conf * strategy = p_data->_conf->_strategy->current();
    uint32_t log_single_deque_length = p_data->_conf->_strategy->current()->log_single_deque_length;

    if (_last_date.empty())
        return;

    std::string yw_tmp;
    get_yearweek(_last_date, yw_tmp);

    if (_last_week == yw_tmp)
    {
        if (!_last_file_date.count(_last_date))
        {
            destroy_date_dict(yw_tmp);

            get_history_wsingle(yw_tmp, _last_files); 

            for (auto ii :  _last_date_dict)
            {
                std::deque<std::shared_ptr<single_vec>> st;
                st.push_back(ii.second);
                complement(st, log_single_deque_length);

                update_date_dict(yw_tmp, ii.first, st);
                update_id_date(yw_tmp, ii.first);
            }
        }
    }
    else
    {
        destroy_date_dict(yw_tmp);
        for (auto ii :  _last_date_dict)
        {
            std::deque<std::shared_ptr<single_vec>> st;
            st.push_back(ii.second);
            complement(st, log_single_deque_length);

            update_date_dict(yw_tmp, ii.first, st);
            update_id_date(yw_tmp, ii.first);
        }
    }

    update_load_search();
}

void history_wsingle_dict::destroy_date_dict(const std::string & week,const  std::string & id)
{
    std::string key;
    creat_key(week, id, key);

    _date_dict.erase(key);
}

void history_wsingle_dict::destroy_date_dict(const std::string & week)
{
    for (auto ii: _id_date_dict)
    {
        destroy_date_dict(week, ii.first);
    }
}

void history_wsingle_dict::update_id_date(const std::string & week, const std::string & id)
{
    auto ii = _id_date_dict.find(id);
    if (ii == _id_date_dict.end())
    {
        std::set<std::string> t_set;
        t_set.insert(week);
        _id_date_dict.insert(std::make_pair(id, t_set));
    }
    else
    {
        ii->second.insert(week);
    }
}

void history_wsingle_dict::update_load_search()
{
    proc_data* p_data = proc_data::instance();
    p_data->_hwsingle_index->idle()->clear();

    update_hwsingle_search();

    p_data->_hwsingle_index->idle_2_current();
}

void history_wsingle_dict::update_hwsingle_search()
{
    proc_data* p_data = proc_data::instance();
    auto hsingle_index = p_data->_hwsingle_index->idle();

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

            auto mm = hsingle_index->id_single.find(id);        
            if (mm == hsingle_index->id_single.end())
            {
                std::deque<std::deque< std::shared_ptr<single_vec>>> tmp_vec;
                 tmp_vec.push_back(kk->second);
                 hsingle_index->id_single.insert(std::make_pair(id, tmp_vec));
            }
            else
            {
                mm->second.push_back(kk->second);
            }

            std::string item_key;
            hsingle_search_item::creat_id_index_key(id, len, item_key);
            hsingle_index->id_idx_date.insert(std::make_pair(item_key, *iii));
            hsingle_search_item::creat_id_date_key(id, *iii, item_key);
            hsingle_index->id_date_idx.insert(std::make_pair(item_key, len));
        }
    }

    strategy_conf * strategy = p_data->_conf->_strategy->current(); 
    
    for (auto ii = hsingle_index->id_single.begin(); ii != hsingle_index->id_single.end(); ii++)
    {
        const std::string & id = ii->first;
        
        auto kk = _id_date_dict.find(id);
        if (kk == _id_date_dict.end())
            continue;

        std::shared_ptr<single_vec> single = std::make_shared<single_vec>(strategy->real_single_scale.size());
        std::deque< std::shared_ptr<single_vec>> dp((int)ii->second.size() + 1, single);


        for (int p = 0; p < (int)ii->second.size(); p++)
        {
            int m = 0;
            std::shared_ptr<single_vec> ss = std::make_shared<single_vec>(strategy->real_single_scale.size());
            while (m < (int)ii->second[p].back()->size())
            {

                ss->at(m).diff = dp[p]->at(m).diff + ii->second[p].back()->at(m).diff;
                m++;
            }

            dp[p+1] = ss;
        }

        hsingle_index->id_sum_single[id] = dp;
    }
}

void history_wsingle_dict::get_history_wsingle(const std::string & week, const std::set<std::string> & files)
{
    for (auto file: files)
    {   
        load_history_wsingle(week, file.c_str());

        if (week == _last_week)
        {   
            _last_file_date.insert(basename((char *)file.c_str()));
        }   
    }
}

int history_wsingle_dict::load()
{
    FILE * fp = fopen(_fullpath, "r");
    ASSERT_WARNING(fp != NULL,"open query dict failed. path[%s]", _fullpath);

    char line[SIZE_LEN_1024];
    char * ptr = NULL, *bname;
    uint32_t query_sign[2];

    std::map<std::string, std::set<std::string>> file_set;
    std::vector<std::string> strVec;

    proc_data* p_data = proc_data::instance();
    strategy_conf * strategy = p_data->_conf->_strategy->current();
    uint32_t i = 0;


    while (fgets(line, sizeof(line), fp)) 
    {
        if('\0' == line[0]){
            continue;
        }
        //line[strlen(line) - 1] = '\0';
        ptr = im_chomp(line);
        if (ptr == NULL || *ptr == '\0'|| *ptr == '#')
            continue;


        SplitString(ptr, '\t', &strVec, SPLIT_MODE_ALL | SPLIT_MODE_TRIM);
        if (strVec.size() < 2)
        {   
            continue;
        }   

        std::set<std::string> files(strVec.begin() + 1, strVec.end());
        file_set.insert(std::make_pair(strVec[0], files));

    }

    fclose(fp);

    if (!file_set.empty())
    {
        _last_week = file_set.rbegin()->first;
        _last_files = file_set.rbegin()->second;
    }

    for (auto ii : file_set)
    {
        get_history_wsingle(ii.first, ii.second);
    }    


    struct stat st;
    stat(_fullpath, &st);
    _last_load = st.st_mtime;

    update_load_search();

    return 0;
}

int history_wsingle_dict::reload()
{
    destroy();

    return load();
}

void history_wsingle_dict::set_path (const char* path)
{
    snprintf(_fullpath, sizeof(_fullpath), "%s", path);
}

bool history_wsingle_dict::need_reload()
{
    struct stat st;

    if (stat(_fullpath, &st) == 0 && S_ISREG(st.st_mode) && st.st_mtime != _last_load)
    {
        return true;
    }

    return false;
}

int history_wsingle_dict::dump()
{
    //FILE * fp = fopen(_dumppath, "w");
    //ASSERT_WARNING(fp != NULL, "history_wsingle_dict dump_data failed, open file [%s] error", _dumppath);

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
    dump_real_wsingle();

    return 0;
}

void history_wsingle_dict::dump_real_wsingle()
{
    char t_buf[SIZE_LEN_512]; 
    char path_buf[SIZE_LEN_512];
    char tmp[SIZE_LEN_512];

    proc_data* p_data = proc_data::instance();
    if (!p_data)
        return ;

    strategy_conf * strategy = p_data->_conf->_strategy->current();
    std::string yw_tmp;
    get_yearweek(*p_data->get_trade_date(), yw_tmp);
    snprintf(t_buf, sizeof(t_buf), "%s/tmp.%s", strategy->real_wsingle_path.c_str(), yw_tmp.c_str());
    snprintf(path_buf, sizeof(t_buf), "%s/%s", strategy->real_wsingle_path.c_str(), yw_tmp.c_str());
    FILE_SYN_WRITE(t_buf, "%s\n", strategy->dump_wsingle_schema.c_str());

    for (auto ii = _id_date_dict.begin(); ii != _id_date_dict.end(); ii++)
    {
        const string & id = ii->first;
        string key;
        creat_key(_last_date, id, key);
        auto kk = _date_dict.find(key);
        if (kk == _date_dict.end())
            continue;

        std::string t_str;   

        std::deque<std::shared_ptr<single_vec> > & st = kk->second; 
        if (!st.size())      
            continue;            

        if (p_data->_block_set->do_check_block(ii->first))
        {
            continue;
        }

        t_str.append(id.c_str());
        t_str.append(1, ';');

        uint32_t i = 0;
        uint32_t k = 0;
        uint32_t log_single_deque_length_max = p_data->_conf->_strategy->current()->log_single_deque_length_max;
        if (st.size() > log_single_deque_length_max && log_single_deque_length_max > 0)
        {
            i = st.size() - log_single_deque_length_max;
        }
        k = i;

        for (uint32_t j =0; j < st.back()->size(); j++)
        {
            for (i = k; i < st.size(); i++) 
            {
                snprintf(tmp, sizeof(tmp), "%d|%d,", st[i]->at(j).in, st[i]->at(j).out);
                t_str.append(tmp);
            }

            t_str.append(1, ';');
        }
        FILE_SYN_WRITE(t_buf, "%s\n", t_str.c_str());
    }

    int ret = rename(t_buf, path_buf);
    if (ret < 0)
    {
        LOG_WARNING("rename t_buf:%s to path_buf:%s, err:%s", t_buf, path_buf, strerror(errno));
    }

    return ;
}

int history_wsingle_dict::destroy()
{
    proc_data* p_data = proc_data::instance();

    {
        std::unordered_map<std::string, std::deque<std::shared_ptr<single_vec>>, str_hasher> tmp;
        _date_dict.swap(tmp);
    }

    {
        std::unordered_map<std::string, std::set<std::string>, str_hasher> tmp;
        _id_date_dict.swap(tmp);
    }

    {
        std::unordered_map<std::string, std::shared_ptr<single_vec>, str_hasher> tmp;
        _last_date_dict.swap(tmp);
    }

    {
        std::set<std::string> tmp;
        _last_files.swap(tmp);
    }

    {
        std::set<std::string> tmp;
        _last_file_date.swap(tmp);
    }

    {
        std::string tmp;
        _last_week.swap(tmp);
    }

    {
        std::string tmp;
        _last_date.swap(tmp);
    }

    return 0;
}


