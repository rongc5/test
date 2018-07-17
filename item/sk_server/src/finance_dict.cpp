#include "finance_dict.h"
#include "base_def.h"
#include "log_helper.h"
#include "ul_sign.h"
#include "common_util.h"
#include "proc_data.h"

finance_dict::finance_dict()
{
}

finance_dict::~finance_dict()
{
    destroy();
}

int finance_dict::init(const char * path, const char * file, const char *dump_dir)
{
    snprintf(_fullpath, sizeof(_fullpath), "%s/%s", path, file);
    snprintf(_dumppath, sizeof(_dumppath), "%s/%s", dump_dir, file);

    destroy();

    return 0;
}

int finance_dict::load()
{
    proc_data* p_data = proc_data::instance();
    FILE * fp = fopen(_fullpath, "r");
    ASSERT_WARNING(fp != NULL,"open query dict failed. path[%s]", _fullpath);

    char line[SIZE_LEN_1024];
    char * ptr = NULL;

    while (fgets(line, 1024, fp)) 
    {
        if('\0' == line[0]){
            continue;
        }
        //line[strlen(line) - 1] = '\0';
        ptr = im_chomp(line);
        if (ptr == NULL || *ptr == '\0'|| *ptr == '#')
            continue;

        std::map<std::string, std::string> ft;

        std::vector<std::string> tmp_vec;
        SplitString(ptr, '\t', &tmp_vec, SPLIT_MODE_ALL);
        if (tmp_vec.size() < 13)
        {
            continue;
        }

        ft["id"] = tmp_vec[0];
        ft["pe"] = tmp_vec[1];
        ft["pb"] = tmp_vec[2];
        ft["cir_value"] = tmp_vec[3];
        ft["value"] = tmp_vec[4];
        ft["mgsy"] = tmp_vec[5];
        ft["mgxj"] = tmp_vec[6];
        ft["mgsygr"] = tmp_vec[7];
        ft["mgxjgr"] = tmp_vec[8];
        ft["zysrgr"] = tmp_vec[9];
        ft["yylrgr"] = tmp_vec[10];
        ft["jlrgr"] = tmp_vec[11];
        ft["time_str"] = tmp_vec[12];
        
        _id_dict.insert(std::make_pair(ft["id"], ft));

        if (has_key<std::string, std::string>(ft, "pb"))
        {
            int pe = atoi(ft["pe"].c_str());
            p_data->_pe_index.idle()->insert(std::make_pair(pe, ft["id"]));
        }

        if (has_key<std::string, std::string>(ft, "pb"))    
        {
            int pb = atoi(ft["pb"].c_str());
            p_data->_pb_index.idle()->insert(std::make_pair(pb, ft["id"]));
        }

        if (has_key<std::string, std::string>(ft, "value"))
        {
            int value = atoi(ft["value"].c_str());
            p_data->_value_index.idle()->insert(std::make_pair(value, ft["id"]));
        }

        if (has_key<std::string, std::string>(ft, "cir_value"))    
        {
            int cir_value = atoi(ft["cir_value"].c_str());
            p_data->_cir_value_index.idle()->insert(std::make_pair(cir_value, ft["id"]));
        }

        if (has_key<std::string, std::string>(ft, "mgxj"))    
        {
            float mgxj = atof(ft["mgxj"].c_str());
            p_data->_mgxj_index.idle()->insert(std::make_pair(mgxj, ft["id"]));
        }

        if (has_key<std::string, std::string>(ft, "mgsy"))    
        {
            float mgsy = atof(ft["mgsy"].c_str());
            p_data->_mgsy_index.idle()->insert(std::make_pair(mgsy, ft["id"]));
        }

        if (has_key<std::string, std::string>(ft, "mgsygr"))    
        {
            float mgsygr = atof(ft["mgsygr"].c_str());
            p_data->_mgsygr_index.idle()->insert(std::make_pair(mgsygr, ft["id"]));
        }

        if (has_key<std::string, std::string>(ft, "mgxjgr"))    
        {
            float mgxjgr = atof(ft["mgxjgr"].c_str());
            p_data->_mgxjgr_index.idle()->insert(std::make_pair(mgxjgr, ft["id"]));
        }

        if (has_key<std::string, std::string>(ft, "zysrgr"))    
        {
            float zysrgr = atof(ft["zysrgr"].c_str());
            p_data->_zysrgr_index.idle()->insert(std::make_pair(zysrgr, ft["id"]));
        }

        if (has_key<std::string, std::string>(ft, "yylrgr"))    
        {
            float yylrgr = atof(ft["yylrgr"].c_str());
            p_data->_yylrgr_index.idle()->insert(std::make_pair(yylrgr, ft["id"]));
        }

        if (has_key<std::string, std::string>(ft, "jlrgr"))    
        {
            float jlrgr = atof(ft["jlrgr"].c_str());
            p_data->_jlrgr_index.idle()->insert(std::make_pair(jlrgr, ft["id"]));
        }
    }

    fclose(fp);
    struct stat st;
    stat(_fullpath, &st);
    _last_load = st.st_mtime;

    {
        p_data->_pe_index.idle_2_current();
        p_data->_pb_index.idle_2_current();
        p_data->_value_index.idle_2_current();
        p_data->_cir_value_index.idle_2_current();
        p_data->_mgxj_index.idle_2_current();
        p_data->_mgsy_index.idle_2_current();
        p_data->_mgsygr_index.idle_2_current();
        p_data->_mgxjgr_index.idle_2_current();
        p_data->_zysrgr_index.idle_2_current();
        p_data->_yylrgr_index.idle_2_current();
        p_data->_jlrgr_index.idle_2_current();
    }

    return 0;
}

int finance_dict::reload()
{
    proc_data* p_data = proc_data::instance();
    {
        std::unordered_map<std::string, std::map<std::string, std::string>, str_hasher> tmp;
        _id_dict.swap(tmp);
    }

    {
        std::multimap<int, std::string> tmp;
        p_data->_pe_index.idle()->swap(tmp);
    }

    {
        std::multimap<int, std::string> tmp;
        p_data->_pb_index.idle()->swap(tmp);
    }

    {
        std::multimap<int, std::string> tmp;
        p_data->_value_index.idle()->swap(tmp);
    }

    {
        std::multimap<int, std::string> tmp;
        p_data->_cir_value_index.idle()->swap(tmp);
    }
    
    {
        std::multimap<float, std::string> tmp;
        p_data->_mgxj_index.idle()->swap(tmp);
    }

    {
        std::multimap<float, std::string> tmp;
        p_data->_mgsy_index.idle()->swap(tmp);
    }

    {
        std::multimap<float, std::string> tmp;
        p_data->_mgsygr_index.idle()->swap(tmp);
    }

    {
        std::multimap<float, std::string> tmp;
        p_data->_mgxjgr_index.idle()->swap(tmp);
    }

    {
        std::multimap<float, std::string> tmp;
        p_data->_zysrgr_index.idle()->swap(tmp);
    }

    {
        std::multimap<float, std::string> tmp;
        p_data->_yylrgr_index.idle()->swap(tmp);
    }

    {
        std::multimap<float, std::string> tmp;
        p_data->_jlrgr_index.idle()->swap(tmp);
    }

    return load();
}

void finance_dict::set_path (const char* path)
{
    snprintf(_fullpath, sizeof(_fullpath), "%s", path);
}

bool finance_dict::need_reload()
{
    struct stat st;

    if (stat(_fullpath, &st) == 0 && S_ISREG(st.st_mode) && st.st_mtime != _last_load)
    {
        return true;
    }

    return false;
}

int finance_dict::dump()
{
    FILE * fp = fopen(_dumppath, "w");
    ASSERT_WARNING(fp != NULL, "finance_dict dump_data failed, open file [%s] error", _dumppath);

    for (auto &p_data: _id_dict)
    {
        for (auto ft:p_data.second)
        {
            fprintf(fp, "%s[%s]\t", ft.first.c_str(), ft.second.c_str());
        }
    }
    fclose(fp);

    return 0;
}

int finance_dict::destroy()
{

    return 0;
}


