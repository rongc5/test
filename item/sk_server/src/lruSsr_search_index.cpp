#include "lruSsr_search_index.h"
#include "sk_util.h"
#include "proc_data.h"


int lruSsr_search_index::get_search_sstr(const std::string & id, const std::string & sstr, int date_index, int date_index_end)
{
    int res_idx;
    auto  search_index = current();

    std::string res;
    res = id + "_" + sstr + "_" + std::to_string(date_index) + "_" + std::to_string(date_index_end);
    
    auto ii = search_index->_mmap_index.find(res);
    if (ii == search_index->_mmap_index.end())
        return -1;

    res_idx = ii->second;

    auto iii = search_index->_mmap_deque.find(res);
    if (iii != search_index->_mmap_deque.end())
    {
        search_index->_dq.erase(iii->second);
        search_index->_mmap_deque.erase(iii);
    }

    search_index->_dq.push_front(res);
    search_index->_mmap_deque[res] = search_index->_dq.begin();


    return res_idx;
}

void lruSsr_search_index::add_search_sstr(const std::string & id, const std::string & sstr, int date_index, int date_index_end, int index)
{
    auto search_index = current();
    proc_data* p_data = proc_data::instance();

    std::string res;
    res = id + "_" + sstr + "_" + std::to_string(date_index) + "_" + std::to_string(date_index_end);
    int lru_ssr_length = p_data->_conf->_strategy->current()->lru_ssr_length > 0? p_data->_conf->_strategy->current()->lru_ssr_length: LRU_SSR_LENGTH_DF;
    

    auto ii = search_index->_mmap_index.find(res);
    if (ii == search_index->_mmap_index.end())
    {
        if ((int)search_index->_dq.size() >= lru_ssr_length)
        {
            search_index->_mmap_index.erase(search_index->_dq.back());
            search_index->_mmap_deque.erase(search_index->_dq.back());
            search_index->_dq.pop_back();
        }

        search_index->_dq.push_front(res);
        search_index->_mmap_deque[res] = search_index->_dq.begin();
        search_index->_mmap_index[res] = index;
    } 
    else if (index != ii->second)
    {

        auto iii = search_index->_mmap_deque.find(res);
        if (iii != search_index->_mmap_deque.end())
        {
            search_index->_dq.erase(iii->second);
            search_index->_mmap_deque.erase(res);
            search_index->_mmap_index.erase(res);
        }
        
        search_index->_dq.push_front(res);
        search_index->_mmap_deque[res] = search_index->_dq.begin();
        search_index->_mmap_index[res] = index;
    }

}


void lruSsr_search_index::destroy_idle()
{
    auto  item = idle();
    item->clear();
}
