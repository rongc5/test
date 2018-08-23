#include "history_single_dict.h"
#include "base_def.h"
#include "log_helper.h"
#include "ul_sign.h"
#include "common_util.h"
#include "proc_data.h"

history_single_dict::history_single_dict()
{
}

history_single_dict::~history_single_dict()
{
    destroy();
}

int history_single_dict::init(const char * path, const char * file, const char *dump_dir)
{
    snprintf(_fullpath, sizeof(_fullpath), "%s/%s", path, file);
    snprintf(_dumppath, sizeof(_dumppath), "%s/%s", dump_dir, file);

    destroy();

    return 0;
}

void history_single_dict::creat_key(const std::string & date, const std::string & id, std::string & key)
{
    key.clear();
    //date_id;
    key.append(date);
    key.append("_");
    key.append(id);

    return;
}

int history_single_dict::load_history_single(const char * file)
{
    if (!file)
    {
        return -1;
    }

    std::string date = basename((char *)file);
    auto ii = _date_dict.find(date);
    if (ii != _date_dict.end())
    {
        LOG_WARNING("file:%s, date:%s has loaded", file, date.c_str());
        return -1;
    }

    _date_index.insert(date);
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

        std::shared_ptr<single_vec> single(new single_vec);
        for (uint32_t i = 1; i < strVec.size() && i+1 < strVec.size(); i += 3)
        {
            single_t hs;

            hs.in = atoi(strVec[i].c_str());
            hs.out = atoi(strVec[i + 1].c_str());
            hs.diff = atoi(strVec[i + 2].c_str());

            single->push_back(hs);
        }


        creat_key(date, strVec[0], key);
        {
            auto ii = _date_dict.find(key);
            if (ii == _date_dict.end())
            {
                _date_dict.insert(std::make_pair(key, single));
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

        {
            for (uint32_t i = 0; i < single.hs_vec.size(); i++)
            {
                if (!single->at(i).diff)
                    break;

                if (i >= p_data->_hsingle_diff_index.idle()->size())
                {
                    std::unordered_map<std::string, std::multimap<int, std::string> > u_map;
                    std::multimap<int, std::string> t_map;

                    t_map.insert(std::make_pair(single->at(i).diff, strVec[0]));
                    u_map.insert(std::make_pair(date, t_map));
                }
                else
                {
                    std::map<std::string, std::multimap<int, std::string> >  & u_map = (*(p_data->_hsingle_diff_index.idle()))[i];

                    auto ii = u_map.find(date);
                    if (ii == u_map.end())
                    {
                        std::multimap<int, std::string> t_map;

                        t_map.insert(std::make_pair(single->at(i).diff, strVec[0]));
                        u_map.insert(std::make_pair(date, t_map));
                    }
                    else
                    {
                        ii->second.insert(std::make_pair(single->at(i).diff, strVec[0]));
                    }
                }
            }
        }

    }

    return 0;
}

void history_single_dict::update_sum_index()
{
    proc_data* p_data = proc_data::instance();
    if (!p_data)
    {
        return;
    }


    std::string key;
    for (auto ii = _date_index.begin(); ii != _date_index.end(); ii++)
    {
        const std::string  & date = *ii;

        for (auto it = _id_date_dict.begin(); it != _id_date_dict.end(); it++)
        {
            const std::string & id = it->first;
            int diff = 0;
            std::shared_ptr<single_vec> hs(new single_vec);

            for (auto iii = it->second.lower_bound(date); iii != it->second.end(); iii++)
            {
                creat_key(*iii, id, key);
                auto tt = _date_dict.find(key);
                if (tt != _date_dict.end())
                {
                    for (uint32_t i = 0; i < tt->second->size(); i++)
                    {
                        if (i >= hs->size())
                        {
                            single_t st;
                            hs->push_back(st);
                        }
                        
                        hs->at(i).diff += tt->second->at(i).diff;
                    }
                }
            }

            {
                creat_key(date, id, key);
                _date_sum_dict.insert(std::make_pair(key, hs));
            }

        }
    }

}

int history_single_dict::load()
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

        load_history_single(ptr);
    }

    fclose(fp);
    struct stat st;
    stat(_fullpath, &st);
    _last_load = st.st_mtime;

    update_sum_index();
    
    proc_data* p_data = proc_data::instance();
    if (p_data)
    {
        p_data->_hsingle_diff_index.idle_2_current();
    }

    return 0;
}

int history_single_dict::reload()
{
    {
        std::unordered_map<std::string, std::shared_ptr<single_vec>, str_hasher> tmp;
        _date_dict.swap(tmp);
    }

    {
        std::unordered_map<std::string, std::shared_ptr<single_vec>, str_hasher> tmp;
        _date_sum_dict.swap(tmp);
    }

    {
        std::unordered_map<std::string, std::set<std::string>, str_hasher> tmp;
        _id_date_dict.swap(tmp);
    }

    {
        std::set<std::string> tmp;
        _date_index.swap(tmp);
    }

    {
        proc_data* p_data = proc_data::instance();
        if (p_data)
        {
            {
                std::vector<std::map<std::string, std::multimap<int, std::string> > > tmp;
                p_data->_hsingle_diff_index.idle()->swap(tmp);
            }
        }
    }
    return load();
}

void history_single_dict::set_path (const char* path)
{
    snprintf(_fullpath, sizeof(_fullpath), "%s", path);
}

bool history_single_dict::need_reload()
{
    struct stat st;

    if (stat(_fullpath, &st) == 0 && S_ISREG(st.st_mode) && st.st_mtime != _last_load)
    {
        return true;
    }

    return false;
}

int history_single_dict::dump()
{
    //FILE * fp = fopen(_dumppath, "w");
    //ASSERT_WARNING(fp != NULL, "history_single_dict dump_data failed, open file [%s] error", _dumppath);

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

int history_single_dict::destroy()
{
    return 0;
}


