#ifndef __history_wquotation_DICT_H_
#define __history_wquotation_DICT_H_

#include "base_reload.h"
#include "sk_def.h"
#include "common_util.h"

class history_wquotation_dict:public reload_inf
{
    public:
        virtual ~history_wquotation_dict();
        history_wquotation_dict();

        virtual int init(const char * path, const char * file, const char *dump_dir);
        virtual int load();
        virtual int reload();
        virtual bool need_reload();
        virtual int dump();
        virtual int destroy();
        void update();

        static void creat_key(const std::string & date, const std::string & id, std::string & key);

        void update_real_wquotation(const std::string & trade_date, const std::string & id, std::shared_ptr<quotation_original> & qt);

        void update_search_index();

        void update_load_search();

        static std::shared_ptr<quotation_t>  original_2_quotation(std::shared_ptr<quotation_original> & original);

    private:
        void get_id_technical(std::shared_ptr<quotation_t> qt, std::deque< std::shared_ptr<quotation_t>> & sum_quotation, int p, std::shared_ptr<technical_t> tt);

        void update_hwquotation_search();

        void update_hwquotation_wave(hquotation_search_item * hqitem);

        void dump_real_wquotation();

        void dump_real_wtechnical();

        void dump_hwquotation_wave();

    private:
        void set_path (const char* path);
       int load_history_wquotation(const std::string & week, const char * file);

        void get_history_wquotation(const std::string & week, const std::set<std::string> & files);

        void update_date_dict(const std::string & week,const  std::string & id, const std::shared_ptr<quotation_t> & qt);

        void update_id_date(const std::string & week, const std::string & id);

        void destroy_date_dict(const std::string & week,const  std::string & id);

        void destroy_date_dict(const std::string & week);

    private:
        char _fullpath[SIZE_LEN_512];
        char _dumppath[SIZE_LEN_512];
        time_t _last_load;
    public:
        // <week_id, quotation> >
        std::unordered_map<std::string, std::shared_ptr<quotation_t>, str_hasher> _date_dict;

        //id, date,
        std::unordered_map<std::string, std::set<std::string>, str_hasher> _id_date_dict;

    private:
        //current date
        std::string _last_week;
        std::set<std::string> _last_files;
        std::set<std::string> _last_file_date;

        std::string _last_date;
        //last week(id,quotation_t)  
        std::unordered_map<std::string, std::shared_ptr<quotation_t>, str_hasher> _last_date_dict;
};


#endif
