#include "technical_dict.h"
#include "base_def.h"
#include "log_helper.h"
#include "ul_sign.h"
#include "common_util.h"
#include "proc_data.h"
#include "base_net_thread.h"
#include "holiday_dict.h"
#include "history_quotation_dict.h"

technical_dict::technical_dict()
{
}

technical_dict::~technical_dict()
{
    destroy();
}

int technical_dict::init(const char * path, const char * file, const char *dump_dir)
{
    snprintf(_fullpath, sizeof(_fullpath), "%s/%s", path, file);
    snprintf(_dumppath, sizeof(_dumppath), "%s/%s", dump_dir, file);

    destroy();

    return 0;
}

int technical_dict::load()
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
        
        std::vector<std::string> tmp_vec;
        SplitString(ptr, '\t', &tmp_vec, SPLIT_MODE_ALL);
        if (!tmp_vec.size())
            tmp_vec.push_back(ptr);

        _date_dict.insert(tmp_vec[0]);
    }

    fclose(fp);
    struct stat st;
    stat(_fullpath, &st);
    _last_load = st.st_mtime;

    update_trade_search_index();

    return 0;
}

void technical_dict::get_technical_date_map(std::map<std::string, std::set<std::string> > & mm)
{
    char t_buf[SIZE_LEN_512];
    char path_buf[SIZE_LEN_512];
    proc_data* p_data = proc_data::instance();
    if (!p_data)
        return ;

    strategy_conf * strategy = p_data->_conf->_strategy->current();
    struct dirent *r;
    DIR *p;
    uint32_t i = 0; 
    std::set<std::string> files;

    p = opendir(strategy->real_quotation_path.c_str());
    if (p == NULL)
    {    
        LOG_WARNING("opendir:%s", strategy->real_quotation_path.c_str());

        return;
    }

    while ((r= readdir(p)) != NULL) 
    {   
        if (r->d_type == DT_REG && strstr(r->d_name, "20"))
        {
            files.insert(r->d_name); 
        }
    }               
    closedir(p);

    i = 0;
    holiday_dict * _holiday_dict = p_data->_holiday_dict->current();
    for (auto ii = files.rbegin(); ii != files.rend() && i < strategy->history_technical_num; ii++)
    {
        std::string date = basename(ii->c_str());
         if (!_holiday_dict->is_trade_date(date.c_str()))
             continue

         std::set<std::string>  dst;
         get_technical_date_list(*ii, files, dst);

        mm.insert(std::make_pair(*ii, dst)); 
        i++;
    }
}

void technical_dict::check_need_update_history_technical()
{
    std::map<std::string, std::set<std::string> mm;
    get_technical_date_map(std::map<std::string, std::set<std::string> > & mm);

    proc_data* p_data = proc_data::instance();
    if (!p_data)
        return ;

    char t_buf[SIZE_LEN_512];
    strategy_conf * strategy = p_data->_conf->_strategy->current();

    for (auto ii = mm.begin(); ii != mm.end(); ii++)
    {
        snprintf(t_buf, sizeof(t_buf), "%s/%s", strategy->history_technical_path.c_str(), ii->first.c_str());
        int d = access(t_buf, F_OK);
            if (!d) 
                continue;

            //pass
        
            std::unordered_map<std::string, std::shared_ptr<quotation_t> date_dict;
            std::unordered_map<std::string, std::set<std::string>, str_hasher> id_date_dict;
        get_quotation_list(mm.second, date_dict, id_date_dict);

        std::unordered_map<std::string, std::shared_ptr<technical_t>, str_hasher>  id_technical;
        get_technical_list(date_dict, id_date_dict, id_technical);
        
        flush_technical_list_2_file(*ii, id_technical);
    }
    
}

void technical_dict::get_technical_date_list(const std::string & date, const std::set<std::string> & files, std::set<std::string> & dst)
{
    
    proc_data* p_data = proc_data::instance();
    if (!p_data)
        return ;

    strategy_conf * strategy = p_data->_conf->_strategy->current();
    auto ii = files.find(date);
    if (ii == files.end())
    {
        return;
    }

    uint32_t i = 0;
    for (; ii != files.rend() && i < strategy->history_technical_cal_length; ii++)
    {
        std::string date = basename(ii->c_str());
         if (!_holiday_dict->is_trade_date(date.c_str()))
             continue

        dst.insert(*ii);
        i++;
    }
}

void technical_dict::get_quotation_list(const  std::set<std::string> & date, std::unordered_map<std::string, std::shared_ptr<quotation_t>, str_hasher> & date_dict, std::unordered_map<std::string, std::set<std::string>, str_hasher> & id_date_dict)
{
    char t_buf[SIZE_LEN_512];
    for (auto & ii: date)
    {
        snprintf(t_buf, sizeof(t_buf), "%s/%s", strategy->real_quotation_path.c_str(), ii.c_str());
        
        load_quoation(t_buf, date_dict, id_date_dict);
    }
}

std::string technical_dict::get_technical_schema()
{
    std::string tmp;
    proc_data* p_data = proc_data::instance();
        if (!p_data)
            return tmp;

    strategy_conf * strategy = p_data->_conf->_strategy->current();

    tmp.append("id;");
    if (strategy->history_technical_num >= 5)
        tmp.append("end_5;");
    if (strategy->history_technical_num >= 10)
        tmp.append("end_10;");
    if (strategy->history_technical_num >= 20)
        tmp.append("end_20;");
    if (strategy->history_technical_num >= 30)
        tmp.append("end_30;");
    if (strategy->history_technical_num >= 60)
        tmp.append("end_60;");

    tmp.append("down_pointer;");
    tmp.append("up_pointer;");
    tmp.append("avg_end;");
    tmp.append("end_avg_end;");
    if (strategy->history_technical_num >= 5)
        tmp.append("end_end_5;");
    if (strategy->history_technical_num >= 10)
        tmp.append("end_end_10;");
    if (strategy->history_technical_num >= 20)
        tmp.append("end_end_20;");
    if (strategy->history_technical_num >= 30)
        tmp.append("end_end_30;");
    if (strategy->history_technical_num >= 60)
        tmp.append("end_end_60;");

    tmp.append("low_end_5;");
    tmp.append("high_end_5;");

    return tmp;
}

void technical_dict::flush_technical_list_2_file(std::string & date, std::unordered_map<std::string, std::shared_ptr<technical_t>, str_hasher> & id_technical)
{
    if (date.empty() || id_technical.empty())
        return;
    
    proc_data* p_data = proc_data::instance();
    if (!p_data)
        return ;

    strategy_conf * strategy = p_data->_conf->_strategy->current();
    char t_buf[SIZE_LEN_512];
    char path_buf[SIZE_LEN_512];

    snprintf(path_buf, sizeof(path_buf), "%s/%s", strategy->history_technical_path.c_str(), strategy->history_technical_file.c_str());
    snprintf(t_buf, sizeof(t_buf), "%s/.tmp_technical", strategy->history_technical_path.c_str());
    FILE * fp = fopen(t_buf, "a");
    if (!fp){ 
        return;
    }

    fprintf(fp, "%s\n", get_technical_schema().c_str());
    for (auto ii = id_technical.begin(); ii != id_technical.end(); ii++)
    {
        std::temp;
        temp.append(ii->first.c_str());
        temp.append(";");

        if (strategy->history_technical_num >= 5)
        {
            temp.append(float_2_str(ii->second->end_5));
            temp.append(";");
        }

        if (strategy->history_technical_num >= 10)
        {
            temp.append(float_2_str(ii->second->end_10));
            temp.append(";");
        }

        if (strategy->history_technical_num >= 20)
        {
            temp.append(float_2_str(ii->second->end_20));
            temp.append(";");
        }

        if (strategy->history_technical_num >= 30)
        {
            temp.append(float_2_str(ii->second.end_30));
            temp.append(";");
        }

        if (strategy->history_technical_num >= 60)
        {
            temp.append(float_2_str(ii->second.end_5));
            temp.append(";");
        }
        temp.append(float_2_str(ii->second.down_pointer));
        temp.append(";");

        temp.append(float_2_str(ii->second.up_pointer));
        temp.append(";");

        temp.append(float_2_str(ii->second.avg_end));
        temp.append(";");

        temp.append(float_2_str(ii->second.end_start));
        temp.append(";");

        temp.append(float_2_str(ii->second.end_avg_end));
        temp.append(";");

        
        if (strategy->history_technical_num >= 5)
        {
            temp.append(float_2_str(ii->second.end_end_5));
            temp.append(";");
        }

        if (strategy->history_technical_num >= 10)
        {
            temp.append(float_2_str(ii->second.end_end_10));
            temp.append(";");
        }


        if (strategy->history_technical_num >= 20)
        {
            temp.append(float_2_str(ii->second.end_end_20));
            temp.append(";");
        }

        if (strategy->history_technical_num >= 30)
        {
            temp.append(float_2_str(ii->second.end_end_30));
            temp.append(";");
        }

        if (strategy->history_technical_num >= 60)
        {
            temp.append(float_2_str(ii->second.end_end_60));
            temp.append(";");
        }
        temp.append(float_2_str(ii->second.low_end_5));
        temp.append(";");

        temp.append(float_2_str(ii->second.high_end_5));
        temp.append(";");


        fprintf(fp, "%s\n", temp.c_str());
    }


    fclose(fp);

    int ret = rename(t_buf, path_buf);
    if (ret < 0)
    {
        LOG_WARNING("rename t_buf:%s to path_buf:%s, err:%s", t_buf, path_buf, strerror(errno));
    }
}

int technical_dict::get_technical_list( std::unordered_map<std::string, std::shared_ptr<quotation_t>, str_hasher> & date_dict, std::unordered_map<std::string, std::set<std::string>, str_hasher> & id_date_dict, std::unordered_map<std::string, std::shared_ptr<technical_t>, str_hasher> & id_technical)
{
    for (auto ii = id_date_dict.begin(); ii != id_date_dict.end(); ii++)
    {
        const std::string & id = ii->first;

        std::shared_ptr<quotation_t> qt = std::make_shared<quotation_t>();
        std::deque<std::shared_ptr<quotation_t>> dp((int)ii->second.size() + 1, qt);

        int p = 0;
        std::shared_ptr<quotation_t> qt;
        for (auto mm = ii->second.begin(); mm != ii->second.end(); mm++)
        {
            std::string key;
            const std::string & date = *mm;
            creat_key(date, id, key);

            auto kk = date_dict.find(key);
            if (kk == date_dict.end())
                continue;

            dp[p+1] = dp[p] + kk->second[p];
            qt = kk->second[p];
            p++;
        }

        if (qt != nullptr)
        {
            std::shared_ptr<technical_t> tt = get_id_technical(qt, dp, ii->second.size());
            id_technical.insert(std::make_pair(id, tt));
        }
    }

    return 0;
}

std::shared_ptr<technical_t> technical_dict::get_id_technical(std::shared_ptr<quotation_t> qt, std::deque< std::shared_ptr<quotation_t>> & sum_quotation, int p)
{
    proc_data* p_data = proc_data::instance();
    if (!p_data)
        return nullptr;

    int len = sum_quotation.size();
    if (p >= len)
        return nullptr;

    strategy_conf * strategy = p_data->_conf->_strategy->current();
    std::shared_ptr<technical_t> tt = std::make_shared<technical_t>();

    char t_buf[SIZE_LEN_1024];
    t_buf[0] = '\0';

    if (p >= 5 && strategy->history_technical_num >= 5)
    {
        tt->end_5 = (sum_quotation[p]->end - sum_quotation[p - 5]->end)/5;
        snprintf(t_buf, sizeof(t_buf), "%.2f", tt->end_5);
        tt->end_5 = atof(t_buf);
    }
    else 
    {
        tt->end_5 = sum_quotation[p]->end/5;
        snprintf(t_buf, sizeof(t_buf), "%.2f", tt->end_5);
        tt->end_5 = atof(t_buf);
    }

    if (p >= 10 && strategy->history_technical_num >= 10)
    {
        tt->end_10 = (sum_quotation[p]->end - sum_quotation[p - 10]->end)/10;
        snprintf(t_buf, sizeof(t_buf), "%.2f", tt->end_10);
        tt->end_10 = atof(t_buf);
    }

    if (p >= 20 && strategy->history_technical_num >= 20)
    {
        tt->end_20 = (sum_quotation[p]->end - sum_quotation[p - 20]->end)/20;
        snprintf(t_buf, sizeof(t_buf), "%.2f", tt->end_20);
        tt->end_20 = atof(t_buf);
    }

    if (p >= 30 && strategy->history_technical_num >= 30)
    {
        tt->end_30 = (sum_quotation[p]->end - sum_quotation[p - 30]->end)/30;
        snprintf(t_buf, sizeof(t_buf), "%.2f", tt->end_30);
        tt->end_30 = atof(t_buf);
    }

    if (p >= 60 && strategy->history_technical_num >= 60)
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

    return tt;
}



int technical_dict::load_quoation(const char * file, std::unordered_map<std::string, std::shared_ptr<quotation_t>, str_hasher> & date_dict, std::unordered_map<std::string,  std::set<std::string>, str_hasher> & id_date_dict)
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
    std::vector<std::string> schemaVec;
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

        auto p = history_quotation_dict::parse_quotation(line, schemaVec);

        if (p.first.empty() && p.second != nullptr) 
        {
            continue;
        }

        const std::string & id = p.first;
        std::shared_ptr<quotation_t> qt;

        qt = history_quotation_dict::original_2_quotation(p.second);

        history_quotation_dict::creat_key(date, id, key);
        auto ii = date_dict.find(key);
        if (ii == date_dict.end())
        {
            date_dict.insert(std::make_pair(key, qt));
        }

        {   
            auto ii = id_date_dict.find(id);
            if (ii == id_date_dict.end())
            {   
                std::set<std::string> t_set;
                t_set.insert(date);
                id_date_dict.insert(std::make_pair(id, t_set));
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

void technical_dict::update_trade_search_index()
{
    proc_data* p_data = proc_data::instance();

    if (!p_data) 
        return;

    std::vector<base_net_thread *> * req_thread = p_data->get_thread("req_thread");
    if (!req_thread)
        return;

    std::shared_ptr<normal_msg>  net_obj(new normal_msg);
    net_obj->_msg_op = NORMAL_MSG_UPDATE_TRADE_DATE;

    ObjId id; 
    id._id = OBJ_ID_THREAD;
    id._thread_index = (*req_thread->begin())->get_thread_index();
    base_net_thread::put_obj_msg(id, net_obj);
}

int technical_dict::reload()
{
    destroy();

    return load();
}

void technical_dict::set_path (const char* path)
{
    snprintf(_fullpath, sizeof(_fullpath), "%s", path);
}

bool technical_dict::need_reload()
{
    struct stat st;

    if (stat(_fullpath, &st) == 0 && S_ISREG(st.st_mode) && st.st_mtime != _last_load)
    {
        return true;
    }

    return false;
}

int technical_dict::dump()
{
    //FILE * fp = fopen(_dumppath, "w");
    //ASSERT_WARNING(fp != NULL, "technical_dict dump_data failed, open file [%s] error", _dumppath);

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

int technical_dict::destroy()
{
    std::unordered_set<std::string, str_hasher> tmp;
    _date_dict.swap(tmp);

    return 0;
}

bool technical_dict::is_trade_date(const char * date)
{
    if (!date)
        return false;

    int year, mon, day, weekday;
    int len = strlen(date);
    if (len < 8)
        return false;

    {
        std::string str(date, 0, 4);
        year = atoi(str.c_str());
        if (year <= 0)
            return false;
    }

    {
        std::string str(date, 4, 2);
        mon = atoi(str.c_str());

        if (mon <=0 || mon > 12)
            return false;
    }


    {
        std::string str(date, 6, 2);
        day = atoi(str.c_str());
        if (day <= 0 || day > 31)
            return false;
    }

    weekday = dayofweek(day, mon, year);
    if (weekday > 5 || weekday < 1)
        return false;

    if (_date_dict.count(date))
        return false;

    return true;
}

void technical_dict::get_trade_date(const std::string & date, std::string & trade_date)
{
    std::string tmp_date;

    trade_date = date;
    int diff = 0;

    while (!is_trade_date(trade_date.c_str()))
    {
        diff++;
        if (diff > 30)// 说明有bug
            break;

        date_change(date, -1 * diff, trade_date);
    }
}

void technical_dict::get_trade_date(std::string & trade_date)
{
    char date[SIZE_LEN_64] = {'\0'};
    time_t now = get_timestr(date, sizeof(date), "%Y%m%d");

    get_trade_date(date, trade_date);
}

