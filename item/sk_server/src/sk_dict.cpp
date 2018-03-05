#include "sk_dict.h"
#include "base_def.h"
#include "log_helper.h"
#include "ul_sign.h"

sk_dict::sk_dict()
{
}

sk_dict::~sk_dict()
{
    destroy();
}

int sk_dict::init(const char * path, uint32_t query_num)
{
    if(_id_dict.create(query_num * 3, query_num * 2) < 0) {
        LOG_WARNING("failed to allocate memory for query_dict");
        return NULL;
    }

    snprintf(_fullpath, sizeof(_fullpath), "%s", path);

    destroy();

    return 0;
}

int sk_dict::load()
{
    FILE * fp = fopen(tmp_path, "r");
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

        sk_t sk;
        sk.reset();

        create_sign_fs64(ptr, strlen(ptr), query_sign, query_sign+1);
        _id_dict.add_node(query_sign, &sk);
    }

    fclose(fp);
    struct stat st;
    stat(_fullpath, &st);
    last_load_ = st.st_mtime;

    return 0;
}

int sk_dict::reload()
{
    _id_dict.renew();
    return load();
}

void sk_dict::set_path (const char* path)
{
    snprintf(_fullpath, sizeof(_fullpath), "%s", path);
}

bool sk_dict::need_reload()
{
    struct stat st;

    if (stat(_fullpath, &st) == 0 && S_ISREG(st.st_mode) && st.st_mtime != _last_load)
    {
        return true;
    }

    return false;
}

int sk_dict::dump()
{

    return 0;
}

int sk_dict::destroy()
{

    return 0;
}


