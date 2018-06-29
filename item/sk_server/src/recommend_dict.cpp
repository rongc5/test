#include "recommend_dict.h"
#include "base_def.h"
#include "log_helper.h"
#include "proc_data.h"

recommend_dict::recommend_dict()
{
}

recommend_dict::~recommend_dict()
{
    destroy();
}

int recommend_dict::init(const char * path, const char * file, const char *dump_dir)
{
    snprintf(_fullpath, sizeof(_fullpath), "%s/%s", path, file);
    snprintf(_dumppath, sizeof(_dumppath), "%s/%s", dump_dir, file);

    destroy();

    return 0;
}

int recommend_dict::load()
{
    FILE * fp = fopen(_fullpath, "r");
    ASSERT_WARNING(fp != NULL,"open recommend_dict failed. path[%s]", _fullpath);

    char line[SIZE_LEN_1024];
    char * ptr = NULL;
    proc_data* p_data = proc_data::instance();

    while (fgets(line, 1024, fp))
    {
        if('\0' == line[0]){
            continue;
        }

        ptr = im_chomp(line);
        if (ptr == NULL || *ptr == '\0'|| *ptr == '#')
            continue;

        std::vector<std::string> tmp_vec;
        SplitString(ptr, '\t', &tmp_vec, SPLIT_MODE_ALL);
        if (tmp_vec.size() < 2)
        {
            continue;
        }

        auto ii = _id_dict.find(tmp_vec[0]);
        if (ii == _id_dict.end())
        {
            std::vector<std::string> t_vec;
            t_vec.push_back(tmp_vec[1]);

            _id_dict.insert(std::make_pair(tmp_vec[0], t_vec));
        }
        else 
        {
            ii->second.push_back(tmp_vec[1]);
        }

        if (p_data)
        {
            auto iii = p_data->_recommend_index.idle()->find(tmp_vec[1]);
            if (iii == p_data->_recommend_index.idle()->end())
            {
                std::vector<std::string> t_vec;
                t_vec.push_back(tmp_vec[0]);
                
                p_data->_recommend_index.idle()->insert(std::make_pair(tmp_vec[1], t_vec));
            }
            else
            {
                iii->second.push_back(tmp_vec[0]);
            }
        }
    }

    struct stat st;
    stat(_fullpath, &st);
    _last_load = st.st_mtime;

    if (p_data)
    {
        p_data->_recommend_index.idle_2_current();
    }

    return 0;
}

int recommend_dict::reload()
{
    destroy();

    return load();
}

void recommend_dict::set_path (const char* path)
{
    snprintf(_fullpath, sizeof(_fullpath), "%s", path);
}

bool recommend_dict::need_reload()
{
    struct stat st;

    if (stat(_fullpath, &st) == 0 && S_ISREG(st.st_mode) && st.st_mtime != _last_load)
    {
        return true;
    }

    return false;
}

int recommend_dict::dump()
{

    return 0;
}

int recommend_dict::destroy()
{
    std::unordered_map<std::string, std::vector<std::string>, str_hasher> tmp;
    _id_dict.swap(tmp);

    proc_data* p_data = proc_data::instance();
    if (p_data)
    {
        std::unordered_map<std::string, std::vector<std::string>, str_hasher> tmp;
        p_data->_recommend_index.idle()->swap(tmp);
    }

    return 0;
}


