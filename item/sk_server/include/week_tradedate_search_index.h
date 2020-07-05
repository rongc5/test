#ifndef __week_tradedate_search_index_H__
#define __week_tradedate_search_index_H__

#include "sk_def.h"
#include "common_util.h"

class week_tradedate_search_index:public ToBufferMgr<std::map<std::string, std::set<std::string> > >
{
    public:
        virtual ~week_tradedate_search_index(){}

        void get_trade_date(const std::string &key, std::set<std::string> & date);

        void add_week_date(const std::string &key, const std::string & date);

        void add_week_date(const std::string &key, std::set<std::string> & date);

        void destroy_idle();
};


#endif

