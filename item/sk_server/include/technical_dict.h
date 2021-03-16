#ifndef __technical_dict_H_
#define __technical_dict_H_

#include "base_reload.h"
#include "sk_def.h"
#include "common_util.h"

class technical_dict:public reload_inf
{
    public:
        virtual ~technical_dict();
        technical_dict();

        virtual int init(const char * path, const char * file, const char *dump_dir);
        virtual int load();
        virtual int reload();
        virtual bool need_reload();
        virtual int dump();
        virtual int destroy();

    public:
        static void check_need_update_history_technical();

        static void get_technical_date_map(std::map<std::string, std::set<std::string> > & mm);

        static void get_technical_date_list(const std::string & date, const std::set<std::string> & files, std::set<std::string> & dst);

        static void get_quotation_list(const  std::set<std::string> & date, std::unordered_map<std::string, std::shared_ptr<quotation_t>, str_hasher> & date_dict, std::unordered_map<std::string, std::set<std::string>, str_hasher> & id_date_dict);

        static int load_quoation(const char * file, std::unordered_map<std::string, std::shared_ptr<quotation_t>, str_hasher> & date_dict, std::unordered_map<std::string, std::set<std::string>, str_hasher> & id_date_dict);

        static int get_technical_list( std::unordered_map<std::string, std::shared_ptr<quotation_t>, str_hasher> & date_dict, std::unordered_map<std::string, std::set<std::string>, str_hasher> & id_date_dict, std::unordered_map<std::string, std::shared_ptr<technical_t>, str_hasher> & id_technical);

        static std::shared_ptr<technical_t> get_id_technical(std::shared_ptr<quotation_t> qt, std::shared_ptr<quotation_t> & sum_quotation, int p);

        static void flush_technical_list_2_file(std::string & date, std::unordered_map<std::string, std::shared_ptr<technical_t>, str_hasher> & id_technical);

        static std::string get_technical_schema();


        bool is_trade_date(const char * date);

        void get_trade_date(const std::string & date, std::string & trade_date);

        void get_trade_date(std::string & trade_date);

        void update_trade_search_index();

    private:
        void set_path (const char* path);

    private:
        char _fullpath[SIZE_LEN_512];
        char _dumppath[SIZE_LEN_512];
        time_t _last_load;
    public:
        // <date>
        //std::unordered_map<std::string, std::string, str_hasher> _date_dict;
        std::unordered_set<std::string, str_hasher> _date_dict;
};



#endif
