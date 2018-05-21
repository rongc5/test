#include "recommend_dict.h"
#include "base_def.h"
#include "log_helper.h"

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
    _cfg.load_cfg(_fullpath);

    struct stat st;
    stat(_fullpath, &st);
    _last_load = st.st_mtime;

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
    _cfg.clear();

    return 0;
}


