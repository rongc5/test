#include "userid_dict.h"
#include "base_def.h"
#include "log_helper.h"
#include "ul_sign.h"
#include "common_util.h"
#include "proc_data.h"
#include "id_dict.h"

userid_dict::userid_dict()
{
}

userid_dict::~userid_dict()
{
    destroy();
}

int userid_dict::init(const char * path, const char * file, const char *dump_dir)
{
    snprintf(_fullpath, sizeof(_fullpath), "%s/%s", path, file);
    snprintf(_dumppath, sizeof(_dumppath), "%s/%s", dump_dir, file);

    destroy();

    return 0;
}


int userid_dict::load_userid_dict(const char * file)
{
    if (!file)
    {
        return -1;
    }

    char date[SIZE_LEN_512];
    get_timestr(date, sizeof(date), "%Y%m%d");
    std::string date_str(date);

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
        
        if (strVec.size() < 2)
        {
            LOG_WARNING("file:%s, format err", file);
            continue;
        }

        if (date_str > strVec[1])
        {
            LOG_WARNING("userid:%s, has date out:%s ", strVec[0].c_str(), strVec[1].c_str());
            continue;
        }


        _userid_date_map.insert(std::make_pair(strVec[0], strVec[1]));
    }
    fclose(fp);

    return 0;
}

bool userid_dict::is_userid_valid(const std::string & userid, const std::string & date)
{
    auto ii = _userid_date_map.find(userid);
    if (ii == _userid_date_map.end())
        return false;

    if (date > ii->second)
        return false;


    return true;
}

int userid_dict::load()
{
    load_userid_dict(_fullpath);

    struct stat st;
    stat(_fullpath, &st);
    _last_load = st.st_mtime;

    return 0;
}

int userid_dict::reload()
{
    destroy();

    return load();
}

void userid_dict::set_path (const char* path)
{
    snprintf(_fullpath, sizeof(_fullpath), "%s", path);
}

bool userid_dict::need_reload()
{
    struct stat st;

    if (stat(_fullpath, &st) == 0 && S_ISREG(st.st_mode) && st.st_mtime != _last_load)
    {
        return true;
    }

    return false;
}

int userid_dict::dump()
{
    //FILE * fp = fopen(_dumppath, "w");
    //ASSERT_WARNING(fp != NULL, "userid_dict dump_data failed, open file [%s] error", _dumppath);

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

int userid_dict::destroy()
{
    proc_data* p_data = proc_data::instance();

    //LOG_TRACE("%p", this);

    {
        std::unordered_map<std::string, std::string, str_hasher> tmp;
        _userid_date_map.swap(tmp);
    }


    return 0;
}
