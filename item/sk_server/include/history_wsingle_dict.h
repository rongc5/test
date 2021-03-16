#ifndef __history_wsingle_DICT_H_
#define __history_wsingle_DICT_H_

#include "base_reload.h"
#include "sk_def.h"
#include "common_util.h"

class history_wsingle_dict:public reload_inf
{
    public:
        virtual ~history_wsingle_dict();
        history_wsingle_dict();

        virtual int init(const char * path, const char * file, const char *dump_dir);
        virtual int load();
        virtual int reload();
        virtual bool need_reload();
        virtual int dump();
        virtual int destroy();
        void update();

        void update_real_wsingle(const std::string & trade_date, const std::string & id, std::shared_ptr<single_vec>  single);

        void update_search_index();

        void update_load_search(); 
    private:
        void update_hwsingle_search();

    private:
        void set_path (const char* path);
        int load_history_wsingle(const std::string & week, const char * file);

        void get_history_wsingle(const std::string & week, const std::set<std::string> & files);

        void update_date_dict(const std::string & week, const std::string & id, const std::deque<std::shared_ptr<single_vec>> & single);
        void update_id_date(const std::string & week, const std::string & id);
        
        void destroy_date_dict(const std::string & week,const  std::string & id);
        void destroy_date_dict(const std::string & week);

        void complement(std::deque<std::shared_ptr<single_vec> > & st, int num);

        void dump_real_wsingle();

    private:
        char _fullpath[SIZE_LEN_512];
        char _dumppath[SIZE_LEN_512];
        time_t _last_load;
    public:
        // <week_id, diff> >
        std::unordered_map<std::string, std::deque<std::shared_ptr<single_vec> >, str_hasher> _date_dict;
        //id, date,
        std::unordered_map<std::string, std::set<std::string>, str_hasher> _id_date_dict;
    private:
        std::string _last_week;
        std::set<std::string> _last_files;
        std::set<std::string> _last_file_date;

        std::string _last_date;
        //last week( date,single_vec) 
        std::unordered_map<std::string, std::shared_ptr<single_vec>, str_hasher> _last_date_dict;

};



#endif
