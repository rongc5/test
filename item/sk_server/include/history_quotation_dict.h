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
        void update();

        static void creat_key(const string & date, const string & id, string & key);

        void update_real_quotation(const string & trade_date, const string & id, shared_ptr<quotation_original> & qt);

        void update_search_index();

        static shared_ptr<quotation_t>  original_2_quotation(shared_ptr<quotation_original> & original);

    private:
        void get_id_technical(shared_ptr<quotation_t> qt, deque< shared_ptr<quotation_t>> & sum_quotation, int p, shared_ptr<technical_t> tt);

        void update_hquotation_search();

        void update_rquotation_search();

        void update_hquotation_wave(hquotation_search_item * hqitem);

        void dump_real_quotation();

        void dump_common_technical();

        void dump_hquotation_wave();

    private:
        void set_path (const char* path);
        int load_history_quoation(const char * file);

    private:
        char _fullpath[SIZE_LEN_512];
        char _dumppath[SIZE_LEN_512];
        time_t _last_load;
    public:
        //<id, quotation_original>
        unordered_map<string, shared_ptr<quotation_original>, str_hasher> _real_dict;

        // <date_id, quotation> >
        unordered_map<string, shared_ptr<quotation_t>, str_hasher> _date_dict;

        //id, date,
        unordered_map<string, set<string>, str_hasher> _id_date_dict;

    private:
        string _last_date;
};


#endif
