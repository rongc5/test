#include "trade_date_dict.h"
#include "base_def.h"
#include "log_helper.h"
#include "ul_sign.h"
#include "common_util.h"

trade_date_dict::trade_date_dict()
{
}

trade_date_dict::~trade_date_dict()
{
    destroy();
}

int trade_date_dict::init(const char * path, const char * file, const char *dump_dir)
{
    snprintf(_fullpath, sizeof(_fullpath), "%s/%s", path, file);
    snprintf(_dumppath, sizeof(_dumppath), "%s/%s", dump_dir, file);

    destroy();

    return 0;
}

int trade_date_dict::load()
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

        auto ii = _date_dict.find(ptr);
        if (ii == _date_dict.end())
        {
            _date_dict.insert(ptr);
        }
    }

    fclose(fp);
    struct stat st;
    stat(_fullpath, &st);
    _last_load = st.st_mtime;

    return 0;
}

int trade_date_dict::reload()
{
    std::set<std::string> tmp;
    _date_dict.swap(tmp);
    return load();
}

void trade_date_dict::set_path (const char* path)
{
    snprintf(_fullpath, sizeof(_fullpath), "%s", path);
}

bool trade_date_dict::need_reload()
{
    struct stat st;

    if (stat(_fullpath, &st) == 0 && S_ISREG(st.st_mode) && st.st_mtime != _last_load)
    {
        return true;
    }

    return false;
}

int trade_date_dict::dump()
{
    //FILE * fp = fopen(_dumppath, "w");
    //ASSERT_WARNING(fp != NULL, "trade_date_dict dump_data failed, open file [%s] error", _dumppath);

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

int trade_date_dict::destroy()
{
    return 0;
}


