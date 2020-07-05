#include "hsingle_search_index.h"
#include "sk_util.h"
#include "proc_data.h"
#include "history_single_dict.h"

bool hsingle_search_index::do_hsingle_diff_le(std::string &name, std::string &value, search_res & search)
{

    hsingle_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();
    strategy_conf * strategy = p_data->_conf->_strategy->current(); 


    //-1:1:10000&-2:2:50000

    std::vector<std::string> t_vec;
    SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
    if (t_vec.size() < 3)
    {
        return false;
    }

    hidex_item hi;
    hi.date_index = t_vec[0];
    hi.single_index = atoi(t_vec[1].c_str());
    hi.single = atoi(t_vec[2].c_str());

    if (hi.single_index >= strategy->real_single_scale.size()) 
    {
        return false;
    }

    if (search.empty())
    {
        for (auto ii = search_index->id_single.begin(); ii != search_index->id_single.end(); ii++)
        {
            search._id_sets.insert(ii->first);
        }
    }

    {
        for (auto k = search._id_sets.begin(); k != search._id_sets.end(); k++)
        {
            auto ii = search_index->id_single.find(*k);
            if (ii == search_index->id_single.end())
                continue;

            std::set<int> date_index_set ;
            std::set<int> date_index_end_set ;

            p_data->get_date_index(ii->first, hi.date_index, hi.date_index_end, search, date_index_set, date_index_end_set);

            const std::deque<std::deque< std::shared_ptr<single_vec>>> &  tt = ii->second;
            int len = tt.size();

            for (auto date_index: date_index_set)
            {
                if (len  < 1 + abs(date_index)) 
                    continue;

                if (tt[len - abs(date_index) - 1].back()->size() <= hi.single_index)
                    continue;

                if (tt[len - abs(date_index) - 1].back()->at(hi.single_index).diff <= hi.single)
                    search.append(name, ii->first, date_index);
            }
        }
    }

    return true;
}

bool hsingle_search_index::do_hsingle_diff_ge(std::string &name, std::string &value, search_res & search)
{

    hsingle_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();
    strategy_conf * strategy = p_data->_conf->_strategy->current(); 

    //-1:1:10000&-2:2:50000

    std::vector<std::string> t_vec;
    SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
    if (t_vec.size() < 3)
        return false;

    hidex_item hi;
    hi.date_index = t_vec[0];
    hi.single_index = atoi(t_vec[1].c_str());
    hi.single = atoi(t_vec[2].c_str());

    if (hi.single_index >= strategy->real_single_scale.size()) 
        return false;


    if (search.empty()) 
    {
        for (auto ii = search_index->id_single.begin(); ii != search_index->id_single.end(); ii++)
        {
            search._id_sets.insert(ii->first);
        }
    }

    {
        for (auto k = search._id_sets.begin(); k != search._id_sets.end(); k++)
        {
            auto ii = search_index->id_single.find(*k);
            if (ii == search_index->id_single.end())
                continue;

            std::set<int> date_index_set ;
            std::set<int> date_index_end_set ;

            p_data->get_date_index(ii->first, hi.date_index, hi.date_index_end, search, date_index_set, date_index_end_set);

            const std::deque<std::deque< std::shared_ptr<single_vec>>> &  tt = ii->second;
            int len = tt.size();
            for (auto date_index: date_index_set)
            {
                if (len  < 1 + abs(date_index)) 
                    continue;

                if (tt[len - abs(date_index) - 1].back()->size() <= hi.single_index)
                    continue;

                if (tt[len - abs(date_index) - 1].back()->at(hi.single_index).diff >= hi.single)
                    search.append(name, ii->first, date_index);
            }
        }
    }

    return true;
}

bool hsingle_search_index::do_hsingle_diff_ge_num_ge(std::string &name, std::string &value, search_res & search)
{

    hsingle_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();
    strategy_conf * strategy = p_data->_conf->_strategy->current(); 

    //-1:0:1:2:10000&-2:0:2:1:50000

    std::vector<std::string> t_vec;
    SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
    if (t_vec.size() < 5)
        return false;

    hidex_item hi;
    hi.date_index = t_vec[0];
    hi.date_index_end = t_vec[1];
    hi.sum = atoi(t_vec[2].c_str());
    hi.single_index = atoi(t_vec[3].c_str());
    hi.single = atoi(t_vec[4].c_str());

    if (hi.single_index >= strategy->real_single_scale.size()) 
        return false;

    int count = 0;
    if (search.empty()) 
    {
        for (auto ii = search_index->id_single.begin(); ii != search_index->id_single.end(); ii++)
        {
            search._id_sets.insert(ii->first);
        }
    }

    {
        for (auto k = search._id_sets.begin(); k != search._id_sets.end(); k++)
        {
            auto ii = search_index->id_single.find(*k);
            if (ii == search_index->id_single.end())
                continue;

            std::set<int> date_index_set ;
            std::set<int> date_index_end_set ;

            p_data->get_date_index(ii->first, hi.date_index, hi.date_index_end, search, date_index_set, date_index_end_set);

            const std::deque<std::deque< std::shared_ptr<single_vec>>> &  tt = ii->second;
            int len = tt.size();
            for (auto date_index: date_index_set)
                for (auto date_index_end: date_index_end_set)
            {
                if (len  < 1 + abs(date_index) || date_index < date_index_end) 
                    continue;

                if (tt[len - abs(date_index) - 1].back()->size() <= hi.single_index)
                    continue;

                count = 0;        
                std::set<int> index;
                for (int k = len - abs(date_index) - 1; k <= len - abs(date_index_end) - 1; k++) 
                {
                    if (tt[k].back()->at(hi.single_index).diff >= hi.single)
                    {
                        count++;
                        index.insert(len - k - 1);
                    }
                }

                if (count >= hi.sum)
                    search.append(name, ii->first, index);
            }
        }
    }

    return true;
}

bool hsingle_search_index::do_hsingle_sum_ge(std::string &name, std::string &value, search_res & search)
{

    hsingle_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();
    strategy_conf * strategy = p_data->_conf->_strategy->current(); 

    //-1:0:1:10000&-2:0:2:50000

    std::vector<std::string> t_vec;
    SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
    if (t_vec.size() < 4)
        return false;

    hidex_item hi;
    hi.date_index = t_vec[0];
    hi.date_index_end = t_vec[1];
    hi.single_index = atoi(t_vec[2].c_str());
    hi.single = atoi(t_vec[3].c_str());

    if (hi.single_index >= strategy->real_single_scale.size()) 
        return false;

    int count = 0;
    if (search.empty())
    {
        for (auto ii = search_index->id_sum_single.begin(); ii != search_index->id_sum_single.end(); ii++)
        {
            search._id_sets.insert(ii->first);
        }
    }

    {
        for (auto k = search._id_sets.begin(); k != search._id_sets.end(); k++)
        {
            auto ii = search_index->id_sum_single.find(*k);
            if (ii == search_index->id_sum_single.end())
                continue;

            std::set<int> date_index_set ;
            std::set<int> date_index_end_set ;

            p_data->get_date_index(ii->first, hi.date_index, hi.date_index_end, search, date_index_set, date_index_end_set);

            const std::deque< std::shared_ptr<single_vec>> &  tt = ii->second;
            int len = tt.size();
            for (auto date_index: date_index_set)
                for (auto date_index_end: date_index_end_set)
            {
                if (len  < 2 + abs(date_index) || date_index < date_index_end) 
                    continue;

                if (tt[len - abs(date_index) - 1]->size() <= hi.single_index)
                    continue;

                count = 0;        
                std::set<int> index;
                int k = len - abs(date_index) - 2;
                count = tt[len - abs(date_index_end) -1]->at(hi.single_index).diff - tt[k]->at(hi.single_index).diff;

                index.insert(date_index);
                index.insert(date_index_end);

                if (count >= hi.single)
                {
                    search.append(name, ii->first, index);
                }
            }
        }
    }

    return true;
}

bool hsingle_search_index::do_hsingle_sum_le(std::string &name, std::string &value, search_res & search)
{

    hsingle_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();
    strategy_conf * strategy = p_data->_conf->_strategy->current(); 

    //-1:0:1:10000&-2:0:2:50000

    std::vector<std::string> t_vec;
    SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
    if (t_vec.size() < 3)
        return false;

    hidex_item hi;
    hi.date_index = t_vec[0];
    hi.date_index_end = t_vec[1];
    hi.single_index = atoi(t_vec[2].c_str());
    hi.single = atoi(t_vec[3].c_str());


    if (hi.single_index >= strategy->real_single_scale.size()) 
        return false;

    int count = 0;
    if (search.empty())
    {
        for (auto ii = search_index->id_sum_single.begin(); ii != search_index->id_sum_single.end(); ii++)
        {
            search._id_sets.insert(ii->first);
        }
    }

    {
        for (auto k = search._id_sets.begin(); k != search._id_sets.end(); k++)
        {
            auto ii = search_index->id_sum_single.find(*k);
            if (ii == search_index->id_sum_single.end())
                continue;

            std::set<int> date_index_set ;
            std::set<int> date_index_end_set ;

            p_data->get_date_index(ii->first, hi.date_index, hi.date_index_end, search, date_index_set, date_index_end_set);

            const std::deque< std::shared_ptr<single_vec>> &  tt = ii->second;
            int len = tt.size();
            for (auto date_index: date_index_set)
                for (auto date_index_end: date_index_end_set)
            {
                if (len  < 2 + abs(date_index) || date_index < date_index_end) 
                    continue;

                if (tt[len - abs(date_index) - 1]->size() <= hi.single_index)
                    continue;

                count = 0;        
                std::set<int> index;
                int k = len - abs(date_index) - 2;
                count = tt[len - abs(date_index_end) -1]->at(hi.single_index).diff - tt[k]->at(hi.single_index).diff;

                index.insert(date_index);
                index.insert(date_index_end);
                if (count <= hi.single)
                {
                    search.append(name, ii->first, index);
                }
            }
        }
    }

    return true;
}


bool hsingle_search_index::do_hsingle_diff2_le(std::string &name, std::string &value, search_res & search)
{

    hsingle_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();
    strategy_conf * strategy = p_data->_conf->_strategy->current(); 

    //-1:-2:1:10000&-2:-3:2:50000

    std::vector<std::string> t_vec;
    SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
    if (t_vec.size() < 4)
        return false;

    hidex_item hi;
    hi.date_index = t_vec[0];
    hi.date_index_end = t_vec[1];
    hi.single_index = atoi(t_vec[2].c_str());
    hi.single = atoi(t_vec[3].c_str());


    if (hi.single_index >= strategy->real_single_scale.size()) 
        return false;

    if (search.empty())
    {
        for (auto ii = search_index->id_single.begin(); ii != search_index->id_single.end(); ii++)
        {
            search._id_sets.insert(ii->first);
        }
    }

    {
        for (auto k = search._id_sets.begin(); k != search._id_sets.end(); k++)
        {
            auto ii = search_index->id_single.find(*k);
            if (ii == search_index->id_single.end())
                continue;

            std::set<int> date_index_set ;
            std::set<int> date_index_end_set ;

            p_data->get_date_index(ii->first, hi.date_index, hi.date_index_end, search, date_index_set, date_index_end_set);

            const std::deque<std::deque< std::shared_ptr<single_vec>>> &  tt = ii->second;
            int len = tt.size();
            for (auto date_index: date_index_set)
                for (auto date_index_end: date_index_end_set)
                {
                    if (len  < 1 + abs(date_index) || len  < 1 + abs(date_index_end) || date_index < date_index_end) 
                    continue;

                if (tt[len - abs(date_index) - 1].back()->size() <= hi.single_index)
                    continue;

                if (tt[len - abs(date_index_end) - 1].back()->size() <= hi.single_index)
                    continue;

                std::set<int> index;
                int sdiff = tt[len - abs(date_index) - 1].back()->at(hi.single_index).diff;
                int ediff = tt[len - abs(date_index_end) - 1].back()->at(hi.single_index).diff;

                index.insert(date_index);
                index.insert(date_index_end);

                if (sdiff <= hi.single + ediff)
                {
                    search.append(name, ii->first, index);
                }
            }
        }
    }

    return true;
}

bool hsingle_search_index::do_hsingle_diff2_ge(std::string &name, std::string &value, search_res & search)
{

    hsingle_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();
    strategy_conf * strategy = p_data->_conf->_strategy->current(); 

    //-1:-2:1:10000&-2:-3:2:50000

    std::vector<std::string> t_vec;
    SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
    if (t_vec.size() < 4)
        return false;

    hidex_item hi;
    hi.date_index = t_vec[0];
    hi.date_index_end = t_vec[1];
    hi.single_index = atoi(t_vec[2].c_str());
    hi.single = atoi(t_vec[3].c_str());

    if (hi.single_index >= strategy->real_single_scale.size()) 
        return false;

    if (search.empty()) 
    {
        for (auto ii = search_index->id_single.begin(); ii != search_index->id_single.end(); ii++)
        {
            search._id_sets.insert(ii->first);
        }
    }

    {
        for (auto k = search._id_sets.begin(); k != search._id_sets.end(); k++)
        {
            auto ii = search_index->id_single.find(*k);
            if (ii == search_index->id_single.end())
                continue;

            std::set<int> date_index_set ;
            std::set<int> date_index_end_set ;

            p_data->get_date_index(ii->first, hi.date_index, hi.date_index_end, search, date_index_set, date_index_end_set);

            const std::deque<std::deque< std::shared_ptr<single_vec>>> &  tt = ii->second;
            int len = tt.size();
            for (auto date_index: date_index_set)
                for (auto date_index_end: date_index_end_set)

            {
                if (len  < 1 + abs(date_index) || len  < 1 + abs(date_index_end) || date_index < date_index_end) 
                    continue;

                if (tt[len - abs(date_index) - 1].back()->size() <= hi.single_index)
                    continue;

                if (tt[len - abs(date_index_end) - 1].back()->size() <= hi.single_index)
                    continue;

                std::set<int> index;
                int sdiff = tt[len - abs(date_index) - 1].back()->at(hi.single_index).diff;
                int ediff = tt[len - abs(date_index_end) - 1].back()->at(hi.single_index).diff;

                
                index.insert(date_index);
                index.insert(date_index_end);
                if (sdiff >= hi.single + ediff)
                {
                    search.append(name, ii->first, index);
                }
            }
        }
    }

    return true;
}
