#include "finance_dict.h"
#include "base_def.h"
#include "log_helper.h"
#include "ul_sign.h"
#include "common_util.h"

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
    FILE * fp = fopen(_fullpath, "r");
    ASSERT_WARNING(fp != NULL,"open query dict failed. path[%s]", _fullpath);

    char line[SIZE_LEN_1024];
    char * ptr = NULL;
    uint32_t query_sign[2];

    while (fgets(line, 1024, fp)) 
    {
        if('\0' == line[0]){
            continue;
        }
        //line[strlen(line) - 1] = '\0';
        ptr = im_chomp(line);
        if (ptr == NULL || *ptr == '\0'|| *ptr == '#')
            continue;

        finance_t ft;

        std::vector<std::string> tmp_vec;
        SplitString(ptr, "\t", &tmp_vec, SPLIT_MODE_ALL);
        if (tmp_vec.size() < 13)
        {
            continue;
        }

        ft.id = tmp_vec[0];
        ft.pe = atof(tmp_vec[1].c_str());
        ft.pb = atof(tmp_vec[2].c_str());
        ft.cir_value = atof(tmp_vec[3].c_str());
        ft.value = atof(tmp_vec[4].c_str());
        ft.mgsy = atof(tmp_vec[5].c_str());
        ft.mgxj = atof(tmp_vec[6].c_str());
        ft.mgsygr = atof(tmp_vec[7].c_str());
        ft.mgxjgr = atof(tmp_vec[8].c_str());
        ft.zysrgr = atof(tmp_vec[9].c_str());
        ft.yylrgr = atof(tmp_vec[10].c_str());
        ft.jlrgr = atof(tmp_vec[11].c_str());
        ft.time_str = tmp_vec[12];
        
        _id_dict.insert(std::make_pair(ft.id, ft));

        {
            auto iii =  _pe_index.find(ft.pe);
            if (iii != _pe_index.end())
            {
                iii->second.push_back(ft.id);
            }   
            else
            {   
                std::vector<std::string> id_vec;
                id_vec.push_back(ft.id);
                _pe_index.insert(std::make_pair(ft.pe, id_vec));
            }
        }

        {
            auto iii =  _pb_index.find(ft.pb);
            if (iii != _pb_index.end())
            {
                iii->second.push_back(ft.id);
            }   
            else
            {   
                std::vector<std::string> id_vec;
                id_vec.push_back(ft.id);
                _pb_index.insert(std::make_pair(ft.pb, id_vec));
            }
        }

        {
            auto iii =  _value_index.find(ft.value);
            if (iii != _value_index.end())
            {
                iii->second.push_back(ft.id);
            }   
            else
            {   
                std::vector<std::string> id_vec;
                id_vec.push_back(ft.id);
                _value_index.insert(std::make_pair(ft.value, id_vec));
            }
        }

        {
            auto iii =  _cir_value_index.find(ft.cir_value);
            if (iii != _cir_value_index.end())
            {
                iii->second.push_back(ft.id);
            }   
            else
            {   
                std::vector<std::string> id_vec;
                id_vec.push_back(ft.id);
                _cir_value_index.insert(std::make_pair(ft.cir_value, id_vec));
            }
        }

        {
            auto iii =  _mgxj_index.find(ft.mgxj);
            if (iii != _mgxj_index.end())
            {
                iii->second.push_back(ft.id);
            }   
            else
            {   
                std::vector<std::string> id_vec;
                id_vec.push_back(ft.id);
                _mgxj_index.insert(std::make_pair(ft.mgxj, id_vec));
            }
        }

        {
            auto iii =  _mgsy_index.find(ft.mgsy);
            if (iii != _mgsy_index.end())
            {
                iii->second.push_back(ft.id);
            }   
            else
            {   
                std::vector<std::string> id_vec;
                id_vec.push_back(ft.id);
                _mgsy_index.insert(std::make_pair(ft.mgsy, id_vec));
            }
        }

        {
            auto iii =  _mgsygr_index.find(ft.mgsygr);
            if (iii != _mgsygr_index.end())
            {
                iii->second.push_back(ft.id);
            }   
            else
            {   
                std::vector<std::string> id_vec;
                id_vec.push_back(ft.id);
                _mgsygr_index.insert(std::make_pair(ft.mgsygr, id_vec));
            }
        }

        {
            auto iii =  _mgxjgr_index.find(ft.mgxjgr);
            if (iii != _mgxjgr_index.end())
            {
                iii->second.push_back(ft.id);
            }   
            else
            {   
                std::vector<std::string> id_vec;
                id_vec.push_back(ft.id);
                _mgxjgr_index.insert(std::make_pair(ft.mgxjgr, id_vec));
            }
        }

        {
            auto iii =  _zysrgr_index.find(ft.zysrgr);
            if (iii != _zysrgr_index.end())
            {
                iii->second.push_back(ft.id);
            }   
            else
            {   
                std::vector<std::string> id_vec;
                id_vec.push_back(ft.id);
                _zysrgr_index.insert(std::make_pair(ft.zysrgr, id_vec));
            }
        }

        {
            auto iii =  _yylrgr_index.find(ft.yylrgr);
            if (iii != _yylrgr_index.end())
            {
                iii->second.push_back(ft.id);
            }   
            else
            {   
                std::vector<std::string> id_vec;
                id_vec.push_back(ft.id);
                _yylrgr_index.insert(std::make_pair(ft.yylrgr, id_vec));
            }
        }
        
        {
            auto iii =  _jlrgr_index.find(ft.jlrgr);
            if (iii != _jlrgr_index.end())
            {
                iii->second.push_back(ft.id);
            }   
            else
            {   
                std::vector<std::string> id_vec;
                id_vec.push_back(ft.id);
                _jlrgr_index.insert(std::make_pair(ft.jlrgr, id_vec));
            }
        }

    }

    fclose(fp);
    struct stat st;
    stat(_fullpath, &st);
    _last_load = st.st_mtime;

    return 0;
}

int finance_dict::reload()
{
    {
        std::unordered_map<std::string, finance_t, str_hasher> tmp;
        _id_dict.swap(tmp);
    }

    {
        std::unordered_map<int, std::vector<std::string> > tmp;
        _pe_index.swap(tmp);
    }

    {
        std::unordered_map<int, std::vector<std::string> > tmp;
        _pb_index.swap(tmp);
    }

    {
        std::unordered_map<int, std::vector<std::string> > tmp;
        _value_index.swap(tmp);
    }

    {
        std::unordered_map<int, std::vector<std::string> > tmp;
        _cir_value_index.swap(tmp);
    }
    
    {
        std::unordered_map<float, std::vector<std::string> > tmp;
        _mgxj_index.swap(tmp);
    }

    {
        std::unordered_map<float, std::vector<std::string> > tmp;
        _mgsy_index.swap(tmp);
    }

    {
        std::unordered_map<float, std::vector<std::string> > tmp;
        _mgsygr_index.swap(tmp);
    }

    {
        std::unordered_map<float, std::vector<std::string> > tmp;
        _mgxjgr_index.swap(tmp);
    }

    {
        std::unordered_map<float, std::vector<std::string> > tmp;
        _zysrgr_index.swap(tmp);
    }

    {
        std::unordered_map<float, std::vector<std::string> > tmp;
        _yylrgr_index.swap(tmp);
    }

    {
        std::unordered_map<float, std::vector<std::string> > tmp;
        _jlrgr_index.swap(tmp);
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
        fprintf(fp,"id[%s]\tpe[%.2f]\tpb[%.2f]\t",p_data.second.id.c_str(), p_data.second.pe, p_data.second.pb);

        fprintf(fp, "cir_value[%.2f]\tvalue[%.2f]\tmgsy[%.2f]\tmgxj[%.2f]\tmgsygr[%.2f]\t", 
                p_data.second.cir_value, 
                p_data.second.value, p_data.second.mgsy, p_data.second.mgxj, p_data.second.mgsygr);

        fprintf(fp, "mgxjgr[%.2f]\tzysrgr[%.2f]\tyylrgr[%.2f]\tjlrgr[%.2f]time_str[%s]\n", 
                p_data.second.mgxjgr, p_data.second.zysrgr, p_data.second.yylrgr, p_data.second.jlrgr, 
                p_data.second.time_str.c_str());
    }
    fclose(fp);

    return 0;
}

int finance_dict::destroy()
{

    return 0;
}


