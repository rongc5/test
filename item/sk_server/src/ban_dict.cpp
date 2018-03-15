#include "ban_dict.h"
#include "base_def.h"
#include "log_helper.h"
#include "ul_sign.h"

ban_dict::ban_dict()
{
}

ban_dict::~ban_dict()
{
    destroy();
}

int ban_dict::init(const char * path, const char * file, uint32_t query_num, const char *dump_dir)
{
    if(_id_dict.create(query_num * 3, query_num * 2) < 0) {
        LOG_WARNING("failed to allocate memory for query_dict");
        return NULL;
    }

    snprintf(_fullpath, sizeof(_fullpath), "%s", path);
    snprintf(_dumppath, sizeof(_dumppath), "%s/%s", dump_dir, file);

    destroy();

    return 0;
}

int ban_dict::load()
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

        ban_t bt;
        vector<string> tmp_vec;
        SplitString(ptr, "\t", &tmp_vec, SPLIT_MODE_ALL);
        if (tmp_vec.size() < 2) 
        {   
            continue;
        }
        bt.id = tmp_vec[0];
        bt.date = tmp_vec[1];

        create_sign_fs64(bt.id.c_str(), bt.id.size(), query_sign, query_sign+1);
        _id_dict.add_node(query_sign, &bt);
    }

    fclose(fp);
    struct stat st;
    stat(_fullpath, &st);
    last_load_ = st.st_mtime;

    return 0;
}

int ban_dict::reload()
{
    _id_dict.renew();
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
    ASSERT_FATAL(fp != NULL, "finance_dict dump_data failed, open file [%s] error", _dumppath);

    uint32_t sign[2];
    finance_t * p_data = NULL;

    inc_dict_t<finance_t>::travel_info_t m_tranverse;
    memset(&m_tranverse, 0, sizeof(m_tranverse));

    for(p_data = current->get_begin(&m_tranverse, sign);
            p_data != current->get_end(&m_tranverse);
            p_data = current->get_next(&m_tranverse, sign)) {

        fprintf(fp, "id[%s]\tdate[%s]\n", p_data->id.c_str(), p_data->date.c_str());
    }
    fclose(fp);

    return 0;
}

int ban_dict::destroy()
{

    return 0;
}


