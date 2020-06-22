#ifndef __HISTORY_QUOTATION_DICT_H_
#define __HISTORY_QUOTATION_DICT_H_

#include "base_reload.h"
#include "sk_def.h"
#include "common_util.h"

class history_quotation_dict:public reload_inf
{
    public:
        virtual ~history_quotation_dict();
        history_quotation_dict();

        virtual int init(const char * path, const char * file, const char *dump_dir);
        virtual int load();
        virtual int reload();
        virtual bool need_reload();
        virtual int dump();
        virtual int destroy();

        static void creat_key(const std::string & date, const std::string & id, std::string & key);

        void update_real_quotation(const std::string & trade_date, const std::string & id, std::shared_ptr<quotation_original> & qt);

        void update_search_index();

        static std::shared_ptr<quotation_t>  original_2_quotation(std::shared_ptr<quotation_original> & original);

    private:
        void get_id_technical(std::shared_ptr<quotation_t> qt, std::deque< std::shared_ptr<quotation_t>> & sum_quotation, int p, std::shared_ptr<technical_t> tt);

        void update_hquotation_search();

        void update_rquotation_search();

        void update_hquotation_wave(hquotation_search_item * hqitem);

    private:
        void set_path (const char* path);
        int load_history_quoation(const char * file);

    private:
        char _fullpath[SIZE_LEN_512];
        char _dumppath[SIZE_LEN_512];
        time_t _last_load;
    public:
        //<id, quotation_original>
        std::unordered_map<std::string, std::shared_ptr<quotation_original>, str_hasher> _real_dict;

        // <date_id, quotation> >
        std::unordered_map<std::string, std::shared_ptr<quotation_t>, str_hasher> _date_dict;

        //id, date,
        std::unordered_map<std::string, std::set<std::string>, str_hasher> _id_date_dict;

    private:
        std::string _last_date;
};


std::shared_ptr<quotation_t> operator + (const std::shared_ptr<quotation_t>  qt, const std::shared_ptr<quotation_t>  mm);

#endif
