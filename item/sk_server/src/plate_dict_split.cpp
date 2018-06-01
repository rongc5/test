#include "plate_dict_split.h"
#include "base_def.h"
#include "log_helper.h"
#include "ul_sign.h"
#include "common_util.h"

plate_dict_split::plate_dict_split()
{
}

plate_dict_split::~plate_dict_split()
{
    destroy();
}

int plate_dict_split::init(const char * path, const char * file, const char *dump_dir)
{
    snprintf(_fullpath, sizeof(_fullpath), "%s/%s", path, file);
    snprintf(_dumppath, sizeof(_dumppath), "%s/%s", dump_dir, file);

    destroy();

    return 0;
}

int plate_dict_split::load()
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

        std::vector<std::string> tmp_vec;
        SplitString(ptr, '\t', &tmp_vec, SPLIT_MODE_ALL);
        if (tmp_vec.size() < 2)
        {
            continue;
        }
        
        std::string id = *(tmp_vec.begin());
        tmp_vec.erase(tmp_vec.begin());

        _id_dict.insert(std::make_pair(id, tmp_vec));

        for (auto iit = tmp_vec.begin(); iit != tmp_vec.begin(); iit++)
        {
            auto iii =  _plate_index.find(*iit);
            if (iii != _plate_index.end())
            {
                iii->second.push_back(id);
            }
            else
            {
                std::vector<std::string> id_vec;
                id_vec.push_back(id);
                _plate_index.insert(std::make_pair(*iit, id_vec));
            }
        }

    }

    fclose(fp);
    struct stat st;
    stat(_fullpath, &st);
    _last_load = st.st_mtime;

    return 0;
}

int plate_dict_split::reload()
{
    {
        std::unordered_map<std::string, std::vector<std::string>, str_hasher> tmp;
        _id_dict.swap(tmp);
    }

    {
        std::unordered_map<std::string, std::vector<std::string>, str_hasher> tmp;
        _plate_index.swap(tmp);
    }

    return load();
}

void plate_dict_split::set_path (const char* path)
{
    snprintf(_fullpath, sizeof(_fullpath), "%s", path);
}

bool plate_dict_split::need_reload()
{
    struct stat st;

    if (stat(_fullpath, &st) == 0 && S_ISREG(st.st_mode) && st.st_mtime != _last_load)
    {
        return true;
    }

    return false;
}

int plate_dict_split::dump()
{
    FILE * fp = fopen(_dumppath, "w");
    ASSERT_WARNING(fp != NULL, "plate_dict_split dump_data failed, open file [%s] error", _dumppath);

    for (auto &p_data: _id_dict)
    {
        fprintf(fp,"id[%s]\t",p_data.first.c_str());

        for (auto ii = p_data.second.begin(); ii != p_data.second.end(); ii++)
        {
            fprintf(fp, "%s", ii->c_str());
        }

        fprintf(fp, "\n");
    }
    fclose(fp);

    return 0;
}

int plate_dict_split::destroy()
{

    return 0;
}


