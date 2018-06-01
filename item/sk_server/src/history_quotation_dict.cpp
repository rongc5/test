#include "history_quotation_dict.h"
#include "base_def.h"
#include "log_helper.h"
#include "ul_sign.h"
#include "common_util.h"

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

int history_quotation_dict::load_history_quoation(const char * file)
{
    if (!file)
    {
        return -1;
    }
    
    std::vector<std::string> strVec;
    SplitString(file, '_', &strVec, SPLIT_MODE_ALL); 
    if (strVec.size() != 2)
    {
        LOG_WARNING("file:%s, SplitString failed", file);
        return -1;
    }

    std::string date = strVec[1];
    auto ii = _id_dict.find(date);
    if (ii != _id_dict.end())
    {
        LOG_WARNING("file:%s, date:%s has loaded", file, date.c_str());
        return -1;
    }

    FILE * fp = fopen(file, "r");
    ASSERT_WARNING(fp != NULL,"open file failed. file[%s]", file);


    char line[SIZE_LEN_1024];
    char * ptr = NULL;
    while (fgets(line, sizeof(line), fp))
    {
        if('\0' == line[0])
        {
            continue;
        }

        ptr = im_chomp(line); 
        if (ptr == NULL || *ptr == '\0'|| *ptr == '#')
            continue;
        
        SplitString(file, '\t', &strVec, SPLIT_MODE_ALL | SPLIT_MODE_TRIM);
        
        if (strVec.size() < 14)
        {
            LOG_WARNING("file:%s, format err", file);
            continue;
        }

        quotation_t qt;

        qt.id = strVec[0];
        qt.start = atof(strVec[1].c_str());
        qt.end = atof(strVec[2].c_str());
        qt.high = atof(strVec[3].c_str());
        qt.low = atof(strVec[4].c_str());
        qt.last_closed = atof(strVec[5].c_str());
        qt.vol = atoi(strVec[6].c_str());
        qt.buy_vol = atoi(strVec[7].c_str());
        qt.sell_vol = atoi(strVec[8].c_str());
        qt.swing = atof(strVec[9].c_str());
        qt.change_rate = atof(strVec[10].c_str());
        qt.range_percent = atof(strVec[11].c_str());
        qt.total_price = atof(strVec[12].c_str());
        qt.blocked = (bool)atoi(strVec[13].c_str());
        

        auto ii = _id_dict.find(date);
        if (ii == _id_dict.end())
        {
            std::unordered_map<std::string, quotation_t, str_hasher> id_map;
            id_map[strVec[1]] = qt;

            _id_dict.insert(std::make_pair(date, id_map));
        }
        else
        {
            ii->second.insert(std::make_pair(strVec[1], qt));
        }
    }

    return 0;
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

        bname = basename(ptr);
        load_history_quoation(bname);
    }

    fclose(fp);
    struct stat st;
    stat(_fullpath, &st);
    _last_load = st.st_mtime;

    return 0;
}

int history_quotation_dict::reload()
{
    std::map<std::string, std::unordered_map<std::string, quotation_t, str_hasher> > tmp;
    _id_dict.swap(tmp);
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
    return 0;
}


