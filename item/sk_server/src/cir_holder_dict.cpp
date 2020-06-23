#include "cir_holder_dict.h"
#include "base_def.h"
#include "log_helper.h"
#include "ul_sign.h"
#include "common_util.h"

cir_holder_dict::cir_holder_dict()
{
}

cir_holder_dict::~cir_holder_dict()
{
    destroy();
}

int cir_holder_dict::init(const char * path, const char * file, const char *dump_dir)
{
    snprintf(_fullpath, sizeof(_fullpath), "%s/%s", path, file);
    snprintf(_dumppath, sizeof(_dumppath), "%s/%s", dump_dir, file);

    destroy();

    return 0;
}

int cir_holder_dict::load()
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
//        tmp_vec.erase(tmp_vec.begin());
        std::set<std::string> ts_set(tmp_vec.begin() + 2, tmp_vec.end());

        std::vector<std::shared_ptr<std::string> > ts_vec;

        for (auto ii = ts_set.begin(); ii != ts_set.end(); ii++)
        {
            std::shared_ptr<std::string> ss(new std::string(*ii));
            auto iii = _cir_holder_set.find(ss);
            if (iii != _cir_holder_set.end())
            {
                ts_vec.push_back(*iii);
            }
            else
            {
                ts_vec.push_back(ss);
                _cir_holder_set.insert(ss);
            }
        }
        
        _id_dict.insert(std::make_pair(id, ts_vec));
    }

    fclose(fp);
    struct stat st;
    stat(_fullpath, &st);
    _last_load = st.st_mtime;

    return 0;
}

int cir_holder_dict::reload()
{
    destroy();

    return load();
}

void cir_holder_dict::set_path (const char* path)
{
    snprintf(_fullpath, sizeof(_fullpath), "%s", path);
}

bool cir_holder_dict::need_reload()
{
    struct stat st;

    if (stat(_fullpath, &st) == 0 && S_ISREG(st.st_mode) && st.st_mtime != _last_load)
    {
        return true;
    }

    return false;
}

int cir_holder_dict::dump()
{
    FILE * fp = fopen(_dumppath, "w");
    ASSERT_WARNING(fp != NULL, "cir_holder_dict dump_data failed, open file [%s] error", _dumppath);

    for (auto &p_data: _id_dict)
    {
        fprintf(fp,"id[%s]\t",p_data.first.c_str());

        for (auto ii = p_data.second.begin(); ii != p_data.second.end(); ii++)
        {
            fprintf(fp, "%s", (*ii)->c_str());
        }

        fprintf(fp, "\n");
    }
    fclose(fp);

    return 0;
}

int cir_holder_dict::destroy()
{
    {
        std::unordered_map<std::string, std::vector<std::shared_ptr<std::string> >, str_hasher, str_equaler> tmp;
        _id_dict.swap(tmp);
    }

    {
        std::unordered_set<std::shared_ptr<std::string>, str_hasher, str_equaler> tmp;
        _cir_holder_set.swap(tmp);
    }

    return 0;
}


