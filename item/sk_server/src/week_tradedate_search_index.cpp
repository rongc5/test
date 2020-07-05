#include "week_tradedate_search_index.h"
#include "sk_util.h"
#include "proc_data.h"


void week_tradedate_search_index::get_trade_date(const std::string &key, std::set<std::string> & date)
{
    auto search_index = current();
    date.clear();

    auto ii = search_index->find(key);
    if (ii != search_index->end())
    {
        date.insert(ii->second.begin(), ii->second.end());
    }
}

void week_tradedate_search_index::add_week_date(const std::string &key, const std::string & date)
{
    auto search_index = idle();
    auto ii = search_index->find(key);
    if (ii != search_index->end())
    {
        ii->second.insert(date);
    }
    else
    {
        std::set<std::string> st;
        st.insert(date);
        search_index->insert(std::make_pair(key, st));
    }
}

void week_tradedate_search_index::add_week_date(const std::string &key, std::set<std::string> & date)
{
    auto search_index = idle();
    auto ii = search_index->find(key);
    if (ii != search_index->end())
    {
        ii->second.insert(date.begin(), date.end());
    }
    else
    {
        search_index->insert(std::make_pair(key, date));
    }
}

void week_tradedate_search_index::destroy_idle()
{
    std::map<std::string, std::set<std::string> > * search_index = idle();
    std::map<std::string, std::set<std::string> > t_week_trade_date;
    search_index->swap(t_week_trade_date);
}
