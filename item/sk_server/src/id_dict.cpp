#include "id_dict.h"
#include "base_def.h"
#include "log_helper.h"

id_dict::id_dict()
{
}

id_dict::~id_dict()
{
    destroy();
}

int id_dict::init(const char * path)
{
    snprintf(_fullpath, sizeof(_fullpath), "%s", path);
    snprintf(_dumppath, sizeof(_dumppath), "%s/%s", dump_dir, file);

    destroy();

    return 0;
}

int id_dict::load()
{
    FILE * fp = fopen(tmp_path, "r");
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

        string str = string(ptr);
        _ua_vec.push_back(str);
    }

    fclose(fp);
    struct stat st;
    stat(_fullpath, &st);
    last_load_ = st.st_mtime;

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

    return 0;
}

