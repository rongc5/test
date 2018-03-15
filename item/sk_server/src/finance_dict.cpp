#include "finance_dict.h"
#include "base_def.h"
#include "log_helper.h"
#include "ul_sign.h"

finance_dict::finance_dict()
{
}

finance_dict::~finance_dict()
{
    destroy();
}

int finance_dict::init(const char * path, const char * file, uint32_t query_num, const char *dump_dir)
{
    if(_id_dict.create(query_num * 3, query_num * 2) < 0) {
        LOG_WARNING("failed to allocate memory for query_dict");
        return NULL;
    }

    snprintf(_fullpath, sizeof(_fullpath), "%s/%s", path, file);
    snprintf(_dumppath, sizeof(_dumppath), "%s/%s", dump_dir, file);

    destroy();

    return 0;
}

int finance_dict::load()
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

        finance_t ft;

        vector<string> tmp_vec;
        SplitString(ptr, "\t", &tmp_vec, SPLIT_MODE_ALL);
        if (tmp_vec.size() < 13)
        {
            continue;
        }

        ft.id = tmp_vec[0];
        ft.pe = atof(tmp_vec[1].c_str());
        ft.pb = atof(tmp_vec[2].c_str());
        ft.cir_value = atof(tmp_vec[3].c_str());
        ft.value = atof(tmp_vec[4].c_str());
        ft.mgsy = atof(tmp_vec[5].c_str());
        ft.mgxj = atof(tmp_vec[6].c_str());
        ft.mgsygr = atof(tmp_vec[7].c_str());
        ft.mgxjgr = atof(tmp_vec[8].c_str());
        ft.zysrgr = atof(tmp_vec[9].c_str());
        ft.yylrgr = atof(tmp_vec[10].c_str());
        ft.jlrgr = atof(tmp_vec[11].c_str());
        ft.time_str = tmp_vec[12];
        create_sign_fs64(ft.id.c_str(), ft.id.size(), query_sign, query_sign+1);
        _id_dict.add_node(query_sign, &ft);
    }

    fclose(fp);
    struct stat st;
    stat(_fullpath, &st);
    last_load_ = st.st_mtime;

    return 0;
}

int finance_dict::reload()
{
    _id_dict.renew();
    return load();
}

void finance_dict::set_path (const char* path)
{
    snprintf(_fullpath, sizeof(_fullpath), "%s", path);
}

bool finance_dict::need_reload()
{
    struct stat st;

    if (stat(_fullpath, &st) == 0 && S_ISREG(st.st_mode) && st.st_mtime != _last_load)
    {
        return true;
    }

    return false;
}

int finance_dict::dump()
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

        fprintf(fp,"sign[%u-%u]\tid[%s]\tpe[%.2f]\tpb[%.2f]\t",sign[0],sign[1],p_data->id,p_data->pe,p_data->pb);
        fprintf(fp, "cir_value[%.2f]\tvalue[%.2f]\tmgsy[%.2f]\tmgxj[%.2f]\tmgsygr[%.2f]\t", p_data->cir_value, 
                p_data->value, p_data->mgsy, p_data->mgxj, p_data->mgsygr);
        fprintf(fp, "mgxjgr[%.2f]\tzysrgr[%.2f]\tyylrgr[%.2f]\tjlrgr[%.2f]time_str[%s]\n", 
                p_data->mgxjgr, p_data->zysrgr, p_data->yylrgr, p_data->jlrgr, p_data->time_str);
    }
    fclose(fp);

    return 0;
}

int finance_dict::destroy()
{

    return 0;
}


