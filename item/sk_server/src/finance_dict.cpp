#include "finance_dict.h"
#include "base_def.h"
#include "log_helper.h"
#include "ul_sign.h"
#include "common_util.h"
#include "proc_data.h"
#include "finance_search_index.h"

finance_dict::finance_dict()
{
}

finance_dict::~finance_dict()
{
    destroy();
}

int finance_dict::init(const char * path, const char * file, const char *dump_dir)
{
    snprintf(_fullpath, sizeof(_fullpath), "%s/%s", path, file);
    snprintf(_dumppath, sizeof(_dumppath), "%s/%s", dump_dir, file);

    destroy();

    return 0;
}

int finance_dict::load()
{
    proc_data* p_data = proc_data::instance();
    FILE * fp = fopen(_fullpath, "r");
    ASSERT_WARNING(fp != NULL,"open query dict failed. path[%s]", _fullpath);

    char line[SIZE_LEN_1024];
    char * ptr = NULL;

    finance_search_item * fsi = p_data->_finance_index->idle();

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
        if (tmp_vec.size() < 9)
        {
            continue;
        }

        std::shared_ptr<finance_t> ft(new finance_t);

        std::string & id =  tmp_vec[0];

        ft->pe = atoi(tmp_vec[1].c_str());
        ft->pb = atoi(tmp_vec[2].c_str());
        ft->cir_value = atoi(tmp_vec[3].c_str());
        ft->value = atoi(tmp_vec[4].c_str());
        ft->mgsy = atof(tmp_vec[5].c_str());
        ft->mgxj = atof(tmp_vec[6].c_str());
        ft->zysrgr = atof(tmp_vec[7].c_str());
        ft->jlrgr = atof(tmp_vec[8].c_str());
        
        fsi->id_finance.insert(std::make_pair(id, ft));

        fsi->pe_index.insert(std::make_pair(ft->pe, id));

        fsi->pb_index.insert(std::make_pair(ft->pb, id));

        fsi->value_index.insert(std::make_pair(ft->value, id));

        fsi->cir_value_index.insert(std::make_pair(ft->cir_value, id));

        fsi->mgxj_index.insert(std::make_pair(ft->mgxj, id));

        fsi->mgsy_index.insert(std::make_pair(ft->mgsy, id));

        fsi->zysrgr_index.insert(std::make_pair(ft->zysrgr, id));

        fsi->jlrgr_index.insert(std::make_pair(ft->jlrgr, id));
    }

    fclose(fp);
    struct stat st;
    stat(_fullpath, &st);
    _last_load = st.st_mtime;

    {
        p_data->_finance_index->idle_2_current();
    }

    return 0;
}

int finance_dict::reload()
{
    destroy();

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
    ASSERT_WARNING(fp != NULL, "finance_dict dump_data failed, open file [%s] error", _dumppath);

    //for (auto &p_data: _id_dict)
    //{
        //fprintf(fp, "%s[%s]\t", ft.first.c_str(), ft.second.c_str());
    //}
    fclose(fp);

    return 0;
}


int finance_dict::destroy()
{
    proc_data* p_data = proc_data::instance();

    p_data->_finance_index->idle()->clear();

    return 0;
}

