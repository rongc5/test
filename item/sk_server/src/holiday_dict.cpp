#include "holiday_dict.h"
#include "base_def.h"
#include "log_helper.h"
#include "ul_sign.h"
#include "common_util.h"
#include "proc_data.h"
#include "base_net_thread.h"

holiday_dict::holiday_dict()
{
}

holiday_dict::~holiday_dict()
{
    destroy();
}

int holiday_dict::init(const char * path, const char * file, const char *dump_dir)
{
    snprintf(_fullpath, sizeof(_fullpath), "%s/%s", path, file);
    snprintf(_dumppath, sizeof(_dumppath), "%s/%s", dump_dir, file);

    destroy();

    return 0;
}

int holiday_dict::load()
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

void holiday_dict::update_trade_search_index()
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

int holiday_dict::reload()
{
    destroy();

    return load();
}

void holiday_dict::set_path (const char* path)
{
    snprintf(_fullpath, sizeof(_fullpath), "%s", path);
}

bool holiday_dict::need_reload()
{
    struct stat st;

    if (stat(_fullpath, &st) == 0 && S_ISREG(st.st_mode) && st.st_mtime != _last_load)
    {
        return true;
    }

    return false;
}

int holiday_dict::dump()
{
    //FILE * fp = fopen(_dumppath, "w");
    //ASSERT_WARNING(fp != NULL, "holiday_dict dump_data failed, open file [%s] error", _dumppath);

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

int holiday_dict::destroy()
{
    std::unordered_set<std::string, str_hasher> tmp;
    _date_dict.swap(tmp);

    return 0;
}

bool holiday_dict::is_trade_date(const char * date)
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

void holiday_dict::get_trade_date(const std::string & date, std::string & trade_date)
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

void holiday_dict::get_trade_date(std::string & trade_date)
{
    char date[SIZE_LEN_64] = {'\0'};
    time_t now = get_timestr(date, sizeof(date), "%Y%m%d");

    get_trade_date(date, trade_date);
}

