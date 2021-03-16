#ifndef __history_technical_dict_H_
#define __history_technical_dict_H_

#include "base_reload.h"
#include "sk_def.h"
#include "common_util.h"

class history_technical_dict:public reload_inf
{
    public:
        virtual ~history_technical_dict();
        history_technical_dict();

        virtual int init(const char * path, const char * file, const char *dump_dir);
        virtual int load();
        virtual int reload();
        virtual bool need_reload();
        virtual int dump();
        virtual int destroy();

    public:
        static void check_need_update_history_technical();

        static void get_technical_date_map(map<string, set<string> > & mm);

        static void get_technical_date_list(const string & date, const set<string> & files, set<string> & dst);

        static void get_quotation_list(const  set<string> & date, unordered_map<string, shared_ptr<quotation_t>, str_hasher> & date_dict, unordered_map<string, set<string>, str_hasher> & id_date_dict);

        static int load_quoation(const char * file, unordered_map<string, shared_ptr<quotation_t>, str_hasher> & date_dict, unordered_map<string, set<string>, str_hasher> & id_date_dict);

        static int get_technical_list( unordered_map<string, shared_ptr<quotation_t>, str_hasher> & date_dict, unordered_map<string, set<string>, str_hasher> & id_date_dict, unordered_map<string, shared_ptr<technical_t>, str_hasher> & id_technical);

        static shared_ptr<technical_t> get_id_technical(shared_ptr<quotation_t> qt, shared_ptr<quotation_t> & sum_quotation, int p);

        static void flush_technical_list_2_file(string & date, unordered_map<string, shared_ptr<technical_t>, str_hasher> & id_technical);

        static string get_technical_schema();




        void update_trade_search_index();

    private:
        void set_path (const char* path);

    private:
        char _fullpath[SIZE_LEN_512];
        char _dumppath[SIZE_LEN_512];
        time_t _last_load;
    public:
        // <date>
        //unordered_map<string, string, str_hasher> _date_dict;
        

        //<date_id, quotation> >
        unordered_map<string, shared_ptr<quotation_t>, str_hasher> _date_dict;
        //id, date
        unordered_map<string, set<string>, str_hasher> _id_date_dict;
};



#endif
