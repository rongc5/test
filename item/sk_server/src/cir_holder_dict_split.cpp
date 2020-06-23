#include "cir_holder_dict_split.h"
#include "base_def.h"
#include "log_helper.h"
#include "ul_sign.h"
#include "common_util.h"
#include "proc_data.h"

cir_holder_dict_split::cir_holder_dict_split()
{
}

cir_holder_dict_split::~cir_holder_dict_split()
{
    destroy();
}

int cir_holder_dict_split::init(const char * path, const char * file, const char *dump_dir)
{
    snprintf(_fullpath, sizeof(_fullpath), "%s/%s", path, file);
    snprintf(_dumppath, sizeof(_dumppath), "%s/%s", dump_dir, file);

    destroy();

    return 0;
}

int cir_holder_dict_split::load()
{
    proc_data* p_data = proc_data::instance();
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

        std::vector<std::string> tmp_vec;
        SplitString(ptr, '\t', &tmp_vec, SPLIT_MODE_ALL);
        if (tmp_vec.size() < 2)
        {
            continue;
        }
        
        std::string id = *(tmp_vec.begin());
        //tmp_vec.erase(tmp_vec.begin());
        std::set<std::string> ts_set(tmp_vec.begin() + 2, tmp_vec.end());

        for (auto iit = ts_set.begin(); iit != ts_set.end(); iit++)
        {
            std::shared_ptr<std::string> ss(new std::string(trim(iit->c_str())));

            auto iii = _cir_holder_set.find(ss);
            if (iii != _cir_holder_set.end())
            {
                p_data->_cir_holder_index->idle()->insert(std::make_pair(*iii, id));
            }
            else
            {
                _cir_holder_set.insert(ss);
                p_data->_cir_holder_index->idle()->insert(std::make_pair(ss, id));
            }
        }
    }

    fclose(fp);
    struct stat st;
    stat(_fullpath, &st);
    _last_load = st.st_mtime;

    p_data->_cir_holder_index->idle_2_current();

    return 0;
}

int cir_holder_dict_split::reload()
{
    destroy();

    return load();
}

void cir_holder_dict_split::set_path (const char* path)
{
    snprintf(_fullpath, sizeof(_fullpath), "%s", path);
}

bool cir_holder_dict_split::need_reload()
{
    struct stat st;

    if (stat(_fullpath, &st) == 0 && S_ISREG(st.st_mode) && st.st_mtime != _last_load)
    {
        return true;
    }

    return false;
}

int cir_holder_dict_split::dump()
{
    return 0;
}

int cir_holder_dict_split::destroy()
{
    proc_data* p_data = proc_data::instance();
    {
        p_data->_cir_holder_index->idle()->clear();
    }

    {   
        std::unordered_set<std::shared_ptr<std::string>, str_hasher, str_equaler> tmp;
        _cir_holder_set.swap(tmp);
    }

    return 0;
}


