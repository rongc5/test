#include "history_main_funds_dict.h"
#include "base_def.h"
#include "log_helper.h"
#include "ul_sign.h"
#include "common_util.h"
#include "proc_data.h"
//#include "hmain_funds_search_index.h"
#include "sk_util.h"

history_main_funds_dict::history_main_funds_dict()
{
}

history_main_funds_dict::~history_main_funds_dict()
{
    destroy();
}

int history_main_funds_dict::init(const char * path, const char * file, const char *dump_dir)
{
    snprintf(_fullpath, sizeof(_fullpath), "%s/%s", path, file);
    snprintf(_dumppath, sizeof(_dumppath), "%s/%s", dump_dir, file);

    destroy();

    return 0;
}


int history_main_funds_dict::load_history_main_funds(const char * file)
{
    if (!file)
    {
        return -1;
    }

    std::string date = basename((char *)file);

    proc_data* p_data = proc_data::instance();
    strategy_conf * strategy = p_data->_conf->_strategy->current();
    uint32_t log_main_funds_deque_length = p_data->_conf->_strategy->current()->log_main_funds_deque_length;

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
        
        
        if (strstr(ptr, "id")) 
        {
            SplitString(ptr, ';', &schemaVec, SPLIT_MODE_ALL | SPLIT_MODE_TRIM);
            continue;
        } else
            SplitString(ptr, ';', &strVec, SPLIT_MODE_ONE);

        if (strVec.size() < 2)
            continue;

        std::deque<std::shared_ptr<main_funds_t> > st;

        std::vector<std::string> tmpVec;

        SplitString(strVec[1].c_str(), '|', &tmpVec, SPLIT_MODE_ALL | SPLIT_MODE_TRIM); 

        uint32_t j = 0;
        if (tmpVec.size() > log_main_funds_deque_length && log_main_funds_deque_length > 0)
        {
            j = tmpVec.size() - log_main_funds_deque_length;
        }

        uint32_t k = 0;
        for (; j < tmpVec.size(); j++, k++)
        {
            auto hs = make_shared<main_funds_t>();
            std::vector<std::string> ttVec;
            SplitString(tmpVec[j].c_str(), ';', &ttVec, SPLIT_MODE_ALL | SPLIT_MODE_TRIM);
            if (ttVec.size() < 9)
            {
                continue;
            }

            hs->force_in = atol(ttVec[0].c_str());
            hs->force_out = atol(ttVec[1].c_str());
            hs->force_diff = atol(ttVec[2].c_str());
            hs->force_dratio = atof(ttVec[3].c_str());

            hs->small_in = atol(ttVec[4].c_str());
            hs->small_out = atol(ttVec[5].c_str());
            hs->small_diff = atol(ttVec[6].c_str());
            hs->small_dratio = atof(ttVec[7].c_str());
            hs->all = atof(ttVec[8].c_str());


            st.push_back(hs);
        }

        creat_key(date, strVec[0], key);
        {
            auto ii = _date_dict.find(key);
            if (ii == _date_dict.end())
            {
                _date_dict.insert(std::make_pair(key, st));
            }
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
    }
    fclose(fp);

    return 0;
}

void history_main_funds_dict::update_real_main_funds(const std::string & trade_date, const std::string & id, std::shared_ptr<main_funds_t>  main_funds)
{
    //std::string key;

    //creat_key(trade_date, id, key);
    proc_data* p_data = proc_data::instance(); 
    strategy_conf * strategy = p_data->_conf->_strategy->current();

    _last_date = trade_date;

    {
        std::string key;

        creat_key(trade_date, id, key);

        auto li = _date_dict.find(key);
        if (li != _date_dict.end())
        {
            if (li->second.empty() || li->second.back()->force_diff != main_funds->force_diff || li->second.back()->small_diff != main_funds->small_diff)
            {
                _date_dict[key].push_back(main_funds);
            }
        }
        else 
        {
            std::deque<std::shared_ptr<main_funds_t> > dq;
            dq.push_back(main_funds);
            _date_dict.insert(std::make_pair(key, dq));
        }

        if (strategy->log_main_funds_deque_length > 0 && _date_dict[key].size() > strategy->log_main_funds_deque_length)
        {
            _date_dict[key].pop_front();
        }
    }


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

void history_main_funds_dict::update_search_index()
{
    proc_data* p_data = proc_data::instance();
    //p_data->_hmain_funds_index->idle()->clear();
    //p_data->_rmain_funds_index->idle()->clear();

    //update_rmain_funds_search();

    //update_hmain_funds_search();

    //p_data->_hmain_funds_index->idle_2_current();
    //p_data->_rmain_funds_index->idle_2_current();
}

void history_main_funds_dict::update_hmain_funds_search()
{
#if 0
    proc_data* p_data = proc_data::instance();
    auto hmain_funds_index = p_data->_hmain_funds_index->idle();

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

            auto mm = hmain_funds_index->id_main_funds.find(id);        
            if (mm == hmain_funds_index->id_main_funds.end())
            {
                std::deque<std::deque< std::shared_ptr<main_funds_vec>>> tmp_vec;
                 tmp_vec.push_back(kk->second);
                 hmain_funds_index->id_main_funds.insert(std::make_pair(id, tmp_vec));
            }
            else
            {
                mm->second.push_back(kk->second);
            }

            std::string item_key;
            hmain_funds_search_item::creat_id_index_key(id, len, item_key);
            hmain_funds_index->id_idx_date.insert(std::make_pair(item_key, *iii));
            hmain_funds_search_item::creat_id_date_key(id, *iii, item_key);
            hmain_funds_index->id_date_idx.insert(std::make_pair(item_key, len));
        }
    }

    strategy_conf * strategy = p_data->_conf->_strategy->current(); 
    
    for (auto ii = hmain_funds_index->id_main_funds.begin(); ii != hmain_funds_index->id_main_funds.end(); ii++)
    {
        const std::string & id = ii->first;
        
        auto kk = _id_date_dict.find(id);
        if (kk == _id_date_dict.end())
            continue;

        std::shared_ptr<main_funds_vec> main_funds = std::make_shared<main_funds_vec>(strategy->real_main_funds_scale.size());
        std::deque< std::shared_ptr<main_funds_vec>> dp((int)ii->second.size() + 1, main_funds);


        for (int p = 0; p < (int)ii->second.size(); p++)
        {
            int m = 0;
            std::shared_ptr<main_funds_vec> ss = std::make_shared<main_funds_vec>(strategy->real_main_funds_scale.size());
            while (m < (int)ii->second[p].back()->size())
            {

                ss->at(m).diff = dp[p]->at(m).diff + ii->second[p].back()->at(m).diff;
                ss->at(m).in = dp[p]->at(m).in + ii->second[p].back()->at(m).in;
                ss->at(m).out = dp[p]->at(m).out + ii->second[p].back()->at(m).out;
                m++;
            }

            dp[p+1] = ss;
        }

        hmain_funds_index->id_sum_main_funds[id] = dp;
    }
#endif
}

void history_main_funds_dict::update_rmain_funds_search()
{
#if 0
    proc_data* p_data = proc_data::instance();
    strategy_conf * strategy = p_data->_conf->_strategy->current();

    auto cur = p_data->_rmain_funds_index->current();
    auto idl = p_data->_rmain_funds_index->idle();

    idl->rmain_funds_diff_index.resize(strategy->real_main_funds_scale.size());

    for (auto ii = _id_date_dict.begin(); ii != _id_date_dict.end(); ii++)
    {
        const std::string & id = ii->first;
        std::string key;
        creat_key(_last_date, id, key);

        auto kk = _date_dict.find(key);
        if (kk == _date_dict.end())
        {
            continue;
        }

        if (p_data->_block_set->do_check_block(id))
        {
            continue;
        }

        {
            std::deque< std::shared_ptr<main_funds_vec>> dq; 
            idl->id_main_funds[id] = dq; 
            auto mm = cur->id_main_funds.find(id);
            if (mm != cur->id_main_funds.end()) 
            {   
                idl->id_main_funds[id] = mm->second;
            }   


            if (idl->id_main_funds[id].empty() || idl->id_main_funds[id].back()->at(0) != kk->second.back()->at(0))
            {
                idl->id_main_funds[id].push_back(kk->second.back());
            }

            if (idl->id_main_funds[id].size() > strategy->real_main_funds_deque_length && strategy->real_main_funds_deque_length > 0)
            {   
                idl->id_main_funds[id].pop_front();
            }
        }

        {
            const auto & main_funds = kk->second.back();
            for (uint32_t i = 0; i< main_funds->size(); i++)
            {   
                if (main_funds->at(i).diff != 0)
                {   
                    idl->rmain_funds_diff_index[i].insert(std::make_pair(main_funds->at(i).diff, id));
                }   
            } 
        }

    }
#endif
}

int history_main_funds_dict::load()
{
    FILE * fp = fopen(_fullpath, "r");
    ASSERT_WARNING(fp != NULL,"open query dict failed. path[%s]", _fullpath);

    char line[SIZE_LEN_1024];
    char * ptr = NULL, *bname;
    uint32_t query_sign[2];

    proc_data* p_data = proc_data::instance();
    strategy_conf * strategy = p_data->_conf->_strategy->current();
    uint32_t i = 0;

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
        load_history_main_funds(filename.c_str());
    } 

    struct stat st;
    stat(_fullpath, &st);
    _last_load = st.st_mtime;

    update_search_index();

    return 0;
}

int history_main_funds_dict::reload()
{
    destroy();

    return load();
}

void history_main_funds_dict::set_path (const char* path)
{
    snprintf(_fullpath, sizeof(_fullpath), "%s", path);
}

bool history_main_funds_dict::need_reload()
{
    struct stat st;

    if (stat(_fullpath, &st) == 0 && S_ISREG(st.st_mode) && st.st_mtime != _last_load)
    {
        return true;
    }

    return false;
}

void history_main_funds_dict::update()
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

    p = opendir(strategy->real_main_funds_path.c_str());
    if (p == NULL)
    {   
        LOG_WARNING("opendir:%s", strategy->real_main_funds_path.c_str());
        return;
    } 

    while ((r= readdir(p)) != NULL)
    {   
        if (r->d_type == DT_REG && start_with(r->d_name, "20"))
        {
            snprintf(t_buf, sizeof(t_buf), "%s/%s", strategy->real_main_funds_path.c_str(), r->d_name);
            files.insert(t_buf);
        }
    } 
    closedir(p);

    snprintf(t_buf, sizeof(t_buf), "%s/.tmp_main_funds", strategy->history_main_funds_path.c_str());
    FILE * fp = fopen(t_buf, "a");
    if (!fp){ 
        return;
    }

    i = 0;
    for (auto ii = files.rbegin(); ii != files.rend() && i < strategy->history_main_funds_num; ii++, i++)
    {
        fprintf(fp, "%s\n", ii->c_str());
    }


    fclose(fp);
    snprintf(path_buf, sizeof(path_buf), "%s/%s", strategy->history_main_funds_path.c_str(), strategy->history_main_funds_file.c_str());
    int ret = rename(t_buf, path_buf);
    if (ret < 0)
    {
        LOG_WARNING("rename t_buf:%s to path_buf:%s, err:%s", t_buf, path_buf, strerror(errno));
    }
}


int history_main_funds_dict::dump()
{
    //FILE * fp = fopen(_dumppath, "w");
    //ASSERT_WARNING(fp != NULL, "history_main_funds_dict dump_data failed, open file [%s] error", _dumppath);

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
    //
    //fclose(fp);
    dump_real_main_funds();

    return 0;
}

void history_main_funds_dict::dump_real_main_funds()
{
    char t_buf[SIZE_LEN_512]; 
    char path_buf[SIZE_LEN_512];
    char tmp[SIZE_LEN_512];

    proc_data* p_data = proc_data::instance();
    if (!p_data)
        return ;

    strategy_conf * strategy = p_data->_conf->_strategy->current();
    snprintf(t_buf, sizeof(t_buf), "%s/tmp.%s", strategy->real_main_funds_path.c_str(), p_data->get_trade_date()->c_str());
    snprintf(path_buf, sizeof(t_buf), "%s/%s", strategy->real_main_funds_path.c_str(), p_data->get_trade_date()->c_str());
    FILE_SYN_WRITE(t_buf, "%s\n", strategy->dump_main_funds_schema.c_str());
    //vector<string> schemaVec;
    //SplitString(strategy->dump_main_funds_schema.c_str(), ';', &schemaVec, SPLIT_MODE_ALL | SPLIT_MODE_TRIM);

    for (auto ii = _id_date_dict.begin(); ii != _id_date_dict.end(); ii++)
    {
        const string & id = ii->first;
        string key;
        creat_key(_last_date, id, key);
        auto kk = _date_dict.find(key);
        if (kk == _date_dict.end())
            continue;

        std::string t_str;   

        std::deque<std::shared_ptr<main_funds_t> > & st = kk->second; 
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
        uint32_t log_main_funds_deque_length_max = p_data->_conf->_strategy->current()->log_main_funds_deque_length_max;
        if (st.size() > log_main_funds_deque_length_max && log_main_funds_deque_length_max > 0)
        {
            i = st.size() - log_main_funds_deque_length_max;
        }
        k = i;

        for (i = k; i < st.size(); i++) 
        {
            t_str.append(int_2_str(st[i]->force_in));
            t_str.append(1, ';');
            t_str.append(int_2_str(st[i]->force_out));
            t_str.append(1, ';');
            t_str.append(int_2_str(st[i]->force_diff));
            t_str.append(1, ';');
            t_str.append(float_2_str(st[i]->force_dratio));
            t_str.append(1, ';');

            t_str.append(int_2_str(st[i]->small_in));
            t_str.append(1, ';');
            t_str.append(int_2_str(st[i]->small_out));
            t_str.append(1, ';');
            t_str.append(int_2_str(st[i]->small_diff));
            t_str.append(1, ';');
            t_str.append(float_2_str(st[i]->small_dratio));
            t_str.append(1, ';');

            t_str.append(int_2_str(st[i]->all));
            t_str.append(1, ';');

            t_str.append(1, '|');
        }

        t_str.append(1, ';');

        FILE_SYN_WRITE(t_buf, "%s\n", t_str.c_str());
    }

    int ret = rename(t_buf, path_buf);
    if (ret < 0)
    {
        LOG_WARNING("rename t_buf:%s to path_buf:%s, err:%s", t_buf, path_buf, strerror(errno));
    }

    return ;
}

int history_main_funds_dict::destroy()
{
    proc_data* p_data = proc_data::instance();

    {
        std::unordered_map<std::string, std::deque<std::shared_ptr<main_funds_t>>, str_hasher> tmp;
        _date_dict.swap(tmp);
    }

    {
        std::unordered_map<std::string, std::set<std::string>, str_hasher> tmp;
        _id_date_dict.swap(tmp);
    }


    return 0;
}


