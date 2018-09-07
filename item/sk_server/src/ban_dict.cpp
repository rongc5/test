#include "ban_dict.h"
#include "base_def.h"
#include "log_helper.h"
#include "ul_sign.h"
#include "common_util.h"

ban_dict::ban_dict()
{
}

ban_dict::~ban_dict()
{
    destroy();
}

int ban_dict::init(const char * path, const char * file, const char *dump_dir)
{
    snprintf(_fullpath, sizeof(_fullpath), "%s", path);
    snprintf(_dumppath, sizeof(_dumppath), "%s/%s", dump_dir, file);

    destroy();

    return 0;
}

int ban_dict::load()
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

        ban_t bt;
        std::vector<std::string> tmp_vec;
        SplitString(ptr, '\t', &tmp_vec, SPLIT_MODE_ALL);
        if (tmp_vec.size() < 2) 
        {   
            continue;
        }
        snprintf(bt.id, sizeof(bt.id), "%s", tmp_vec[0].c_str());
        snprintf(bt.date, sizeof(bt.date), "%s", tmp_vec[1].c_str());

        auto ii = _id_dict.find(bt.id);
        if (ii == _id_dict.end())
        {
            std::vector<ban_t> ban_v;
            ban_v.push_back(bt);
            _id_dict.insert(std::make_pair(bt.id, ban_v));
        }
        else
        {
            ii->second.push_back(bt);
        }
    }

    fclose(fp);
    struct stat st;
    stat(_fullpath, &st);
    _last_load = st.st_mtime;

    return 0;
}

int ban_dict::reload()
{
    destroy();

    return load();
}

void ban_dict::set_path (const char* path)
{
    snprintf(_fullpath, sizeof(_fullpath), "%s", path);
}

bool ban_dict::need_reload()
{
    struct stat st;

    if (stat(_fullpath, &st) == 0 && S_ISREG(st.st_mode) && st.st_mtime != _last_load)
    {
        return true;
    }

    return false;
}

int ban_dict::dump()
{
    FILE * fp = fopen(_dumppath, "w");
    ASSERT_WARNING(fp != NULL, "finance_dict dump_data failed, open file [%s] error", _dumppath);

    for (const auto & u :  _id_dict)
    {
        for (const auto & ii: u.second)
            fprintf(fp, "id[%s]\tdate[%s]\n", ii.id, ii.date);
    }
    fclose(fp);

    return 0;
}

int ban_dict::destroy()
{
    std::unordered_map<std::string, std::vector<ban_t>, str_hasher> tmp;
    _id_dict.swap(tmp);

    return 0;
}


