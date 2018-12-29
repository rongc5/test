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


        std::vector<std::string> tmp_vec;
        SplitString(ptr, '\t', &tmp_vec, SPLIT_MODE_ALL);
        if (tmp_vec.size() < 13)
        {
            continue;
        }

        std::shared_ptr<finance_t> ft(new finance_t);

        snprintf(ft->id, sizeof(ft->id), "%s", tmp_vec[0].c_str());
        ft->pe = atoi(tmp_vec[1].c_str());
        ft->pb = atoi(tmp_vec[2].c_str());
        ft->cir_value = atoi(tmp_vec[3].c_str());
        ft->value = atoi(tmp_vec[4].c_str());
        ft->mgsy = atof(tmp_vec[5].c_str());
        ft->mgxj = atof(tmp_vec[6].c_str());
        ft->mgsygr = atof(tmp_vec[7].c_str());
        ft->mgxjgr = atof(tmp_vec[8].c_str());
        ft->zysrgr = atof(tmp_vec[9].c_str());
        ft->yylrgr = atof(tmp_vec[10].c_str());
        ft->jlrgr = atof(tmp_vec[11].c_str());
        
        p_data->_finance_dict_index.idle()->insert(std::make_pair(ft->id, ft));

        p_data->_pe_index->idle()->insert(std::make_pair(ft->pe, ft->id));

        p_data->_pb_index->idle()->insert(std::make_pair(ft->pb, ft->id));

        p_data->_value_index->idle()->insert(std::make_pair(ft->value, ft->id));

        p_data->_cir_value_index->idle()->insert(std::make_pair(ft->cir_value, ft->id));

        p_data->_mgxj_index->idle()->insert(std::make_pair(ft->mgxj, ft->id));

        p_data->_mgsy_index->idle()->insert(std::make_pair(ft->mgsy, ft->id));

        p_data->_mgsygr_index->idle()->insert(std::make_pair(ft->mgsygr, ft->id));

        p_data->_mgxjgr_index->idle()->insert(std::make_pair(ft->mgxjgr, ft->id));

        p_data->_zysrgr_index->idle()->insert(std::make_pair(ft->zysrgr, ft->id));

        p_data->_yylrgr_index->idle()->insert(std::make_pair(ft->yylrgr, ft->id));

        p_data->_jlrgr_index->idle()->insert(std::make_pair(ft->jlrgr, ft->id));
    }

    fclose(fp);
    struct stat st;
    stat(_fullpath, &st);
    _last_load = st.st_mtime;

    {
        p_data->_pe_index->idle_2_current();
        p_data->_pb_index->idle_2_current();
        p_data->_value_index->idle_2_current();
        p_data->_cir_value_index->idle_2_current();
        p_data->_mgxj_index->idle_2_current();
        p_data->_mgsy_index->idle_2_current();
        p_data->_mgsygr_index->idle_2_current();
        p_data->_mgxjgr_index->idle_2_current();
        p_data->_zysrgr_index->idle_2_current();
        p_data->_yylrgr_index->idle_2_current();
        p_data->_jlrgr_index->idle_2_current();
        p_data->_finance_dict_index.idle_2_current();
    }

    return 0;
}

int finance_dict::reload()
{
    destroy();

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

    //for (auto &p_data: _id_dict)
    //{
        //fprintf(fp, "%s[%s]\t", ft.first.c_str(), ft.second.c_str());
    //}
    fclose(fp);

    return 0;
}


int finance_dict::destroy()
{
    proc_data* p_data = proc_data::instance();
    {
        std::multimap<int, std::string> tmp;
        p_data->_pe_index->idle()->swap(tmp);
    }

    {
        std::multimap<int, std::string> tmp;
        p_data->_pb_index->idle()->swap(tmp);
    }

    {
        std::multimap<int, std::string> tmp;
        p_data->_value_index->idle()->swap(tmp);
    }

    {
        std::multimap<int, std::string> tmp;
        p_data->_cir_value_index->idle()->swap(tmp);
    }
    
    {
        std::multimap<float, std::string> tmp;
        p_data->_mgxj_index->idle()->swap(tmp);
    }

    {
        std::multimap<float, std::string> tmp;
        p_data->_mgsy_index->idle()->swap(tmp);
    }

    {
        std::multimap<float, std::string> tmp;
        p_data->_mgsygr_index->idle()->swap(tmp);
    }

    {
        std::multimap<float, std::string> tmp;
        p_data->_mgxjgr_index->idle()->swap(tmp);
    }

    {
        std::multimap<float, std::string> tmp;
        p_data->_zysrgr_index->idle()->swap(tmp);
    }

    {
        std::multimap<float, std::string> tmp;
        p_data->_yylrgr_index->idle()->swap(tmp);
    }

    {
        std::multimap<float, std::string> tmp;
        p_data->_jlrgr_index->idle()->swap(tmp);
    }

    {
        std::unordered_map<std::string, std::shared_ptr<finance_t>, str_hasher> tmp;
        p_data->_finance_dict_index.idle()->swap(tmp);
    }

    return 0;
}

