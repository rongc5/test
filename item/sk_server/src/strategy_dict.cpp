#include "strategy_dict.h"
#include "base_def.h"
#include "log_helper.h"

strategy_dict::strategy_dict()
{
}

strategy_dict::~strategy_dict()
{
    destroy();
}

int strategy_dict::init(const char * path, uint32_t ua_num)
{
    snprintf(_fullpath, sizeof(_fullpath), "%s", path);

    destroy();

    return 0;
}

int strategy_dict::load()
{
    _cfg.load_cfg(_fullpath);

    struct stat st;
    stat(_fullpath, &st);
    last_load_ = st.st_mtime;

    return 0;
}

int strategy_dict::reload()
{
    destroy();
    return load();
}

void strategy_dict::set_path (const char* path)
{
    snprintf(_fullpath, sizeof(_fullpath), "%s", path);
}

bool strategy_dict::need_reload()
{
    struct stat st;

    if (stat(_fullpath, &st) == 0 && S_ISREG(st.st_mode) && st.st_mtime != _last_load)
    {
        return true;
    }

    return false;
}

int strategy_dict::dump()
{

    return 0;
}

int strategy_dict::destroy()
{
    _cfg.clear();

    return 0;
}


