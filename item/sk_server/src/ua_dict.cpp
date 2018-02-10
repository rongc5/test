#include "ua_dict.h"
#include "base_def.h"
#include "log_helper.h"

int ua_dict::init(const char * path, uint32_t ua_num)
{
    snprintf(_fullpath, sizeof(_fullpath), "%s", path);

    return 0;
}

int ua_dict::load()
{

    struct stat st;
    stat(_fullpath, &st);
    last_load_ = st.st_mtime;

    return 0;
}

int ua_dict::reload()
{
    load();
    return 0;
}

void ua_dict::set_path (const char* path)
{
    snprintf(_fullpath, sizeof(_fullpath), "%s", path);
}

bool ua_dict::need_reload()
{
    struct stat st;

    if (stat(_fullpath, &st) == 0 && S_ISREG(st.st_mode) && st.st_mtime != _last_load)
    {
        return true;
    }

    return false;
}

int ua_dict::dump()
{

    return 0;
}

int ua_dict::destroy()
{

    return 0;
}


