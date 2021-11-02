#include "id_dict.h"
#include "base_def.h"
#include "log_helper.h"
#include "common_util.h"

id_dict::id_dict()
{
}

id_dict::~id_dict()
{
    destroy();
}

int id_dict::init(const char * path, const char * file, const char *dump_dir)
{
    snprintf(_fullpath, sizeof(_fullpath), "%s/%s", path, file);
    snprintf(_dumppath, sizeof(_dumppath), "%s/%s", dump_dir, file);

    destroy();

    return 0;
}

int id_dict::load()
{
    FILE * fp = fopen(_fullpath, "r");
    ASSERT_WARNING(fp != NULL,"open query dict failed. path[%s]", _fullpath);

    char line[SIZE_LEN_1024];
    char * ptr = NULL;

    while (fgets(line, 1024, fp)) 
    {
        if('\0' == line[0]){
            continue;
        }
        line[strlen(line) - 1] = '\0';
        ptr = im_chomp(line);
        if (ptr == NULL || *ptr == '\0'|| *ptr == '#')
            continue;

        std::vector<std::string> tmp_vec;
        SplitString(ptr, ';', &tmp_vec, SPLIT_MODE_ALL);
        if (tmp_vec.size() >= 2)
        {
            _id_vec.push_back(tmp_vec[0]);
            _id_name_map[tmp_vec[0]] = tmp_vec[1];
        }
        else
        {
            _id_vec.push_back(std::string(ptr));
        }
    }

    fclose(fp);
    struct stat st;
    stat(_fullpath, &st);
    _last_load = st.st_mtime;

    return 0;
}

int id_dict::reload()
{
    destroy();
    return load();
}

void id_dict::set_path (const char* path)
{
    snprintf(_fullpath, sizeof(_fullpath), "%s", path);
}

bool id_dict::need_reload()
{
    struct stat st;

    if (stat(_fullpath, &st) == 0 && S_ISREG(st.st_mode) && st.st_mtime != _last_load)
    {
        return true;
    }

    return false;
}

int id_dict::dump()
{

    return 0;
}

int id_dict::destroy()
{
    _id_vec.clear();
    _id_name_map.clear();

    return 0;
}

size_t id_dict::size()
{
    return _id_vec.size();
}

