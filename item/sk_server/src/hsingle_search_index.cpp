#include "hsingle_search_index.h"
#include "sk_util.h"
#include "proc_data.h"
#include "history_single_dict.h"

bool hsingle_search_index::do_hsingle_diff_le(std::string &key, std::string &value, std::set<std::string> & search)
{
    std::vector<hidex_item> vec_idex;
    hsingle_search_item * search_index = current();
    SETS_OP_TRPE tmp_ot;
    proc_data* p_data = proc_data::instance();
    strategy_conf * strategy = p_data->_conf->_strategy->current(); 

    std::vector<std::string> tmp_vec;

    if (strstr(value.c_str(), "|")) 
    {
        SplitString(value.c_str(), '|', &tmp_vec, SPLIT_MODE_ALL);
        tmp_ot = SETS_OP_UNION;
    }
    else 
    {
        SplitString(value.c_str(), ';', &tmp_vec, SPLIT_MODE_ALL); 
        tmp_ot = SETS_OP_INTERSECTION;
    }

    if (!tmp_vec.size())
        tmp_vec.push_back(value);

    //-1:1:10000&-2:2:50000
    
    for (uint32_t i = 0; i< tmp_vec.size(); i++)
    {
        std::vector<std::string> t_vec;
        SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 3)
            continue;

        hidex_item hi;
        hi.date_index = atoi(t_vec[0].c_str());
        hi.single_index = atoi(t_vec[1].c_str());
        hi.single = atoi(t_vec[2].c_str());
        vec_idex.push_back(hi);

        if (hi.single_index >= strategy->real_single_scale.size()) 
            return false;

    }

    int cnt = 0;
    std::set<std::string> res;
    if (search.empty())
    {
        for (auto ii = search_index->id_single.begin(); ii != search_index->id_single.end(); ii++)
        {
            const std::deque< std::shared_ptr<single_vec>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++)
            {
                if (len  < 1 + abs(vec_idex[i].date_index)) 
                    continue;

                if (tt[len - abs(vec_idex[i].date_index) - 1]->size() <= vec_idex[i].single_index)
                    continue;

                if (tt[len - abs(vec_idex[i].date_index) - 1]->at(vec_idex[i].single_index).diff <= vec_idex[i].single)
                    cnt++;
            }

            if (tmp_ot == SETS_OP_INTERSECTION && cnt == (int)vec_idex.size())
                res.insert(ii->first);  
            else if (tmp_ot == SETS_OP_UNION && cnt)
                res.insert(ii->first);  
        }
    }
    else
    {
        for (auto k = search.begin(); k != search.end(); k++)
        {
            auto ii = search_index->id_single.find(*k);
            if (ii == search_index->id_single.end())
                continue;

            const std::deque< std::shared_ptr<single_vec>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++)
            {
                if (len  < 1 + abs(vec_idex[i].date_index)) 
                    continue;

                if (tt[len - abs(vec_idex[i].date_index) - 1]->size() <= vec_idex[i].single_index)
                    continue;

                if (tt[len - abs(vec_idex[i].date_index) - 1]->at(vec_idex[i].single_index).diff <= vec_idex[i].single)
                    cnt++;
            }

            if (tmp_ot == SETS_OP_INTERSECTION && cnt == (int)vec_idex.size())
                res.insert(ii->first);  
            else if (tmp_ot == SETS_OP_UNION && cnt)
                res.insert(ii->first);  
        }
    }
    
    res = search;

    return true;
}

bool hsingle_search_index::do_hsingle_diff_ge(std::string &key, std::string &value, std::set<std::string> & search)
{
    std::vector<hidex_item> vec_idex;
    hsingle_search_item * search_index = current();
    SETS_OP_TRPE tmp_ot;
    proc_data* p_data = proc_data::instance();
    strategy_conf * strategy = p_data->_conf->_strategy->current(); 

    std::vector<std::string> tmp_vec;

    if (strstr(value.c_str(), "|")) 
    {
        SplitString(value.c_str(), '|', &tmp_vec, SPLIT_MODE_ALL);
        tmp_ot = SETS_OP_UNION;
    }
    else 
    {
        SplitString(value.c_str(), ';', &tmp_vec, SPLIT_MODE_ALL); 
        tmp_ot = SETS_OP_INTERSECTION;
    }

    if (!tmp_vec.size())
        tmp_vec.push_back(value);

    //-1:1:10000&-2:2:50000
    
    for (uint32_t i = 0; i< tmp_vec.size(); i++)
    {
        std::vector<std::string> t_vec;
        SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 3)
            continue;

        hidex_item hi;
        hi.date_index = atoi(t_vec[0].c_str());
        hi.single_index = atoi(t_vec[1].c_str());
        hi.single = atoi(t_vec[2].c_str());
        vec_idex.push_back(hi);

        if (hi.single_index >= strategy->real_single_scale.size()) 
            return false;

    }

    int cnt = 0;
    std::set<std::string> res;
    if (search.empty()) 
    {
        for (auto ii = search_index->id_single.begin(); ii != search_index->id_single.end(); ii++)
        {
            const std::deque< std::shared_ptr<single_vec>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++)
            {
                if (len  < 1 + abs(vec_idex[i].date_index)) 
                    continue;

                if (tt[len - abs(vec_idex[i].date_index) - 1]->size() <= vec_idex[i].single_index)
                    continue;

                if (tt[len - abs(vec_idex[i].date_index) - 1]->at(vec_idex[i].single_index).diff >= vec_idex[i].single)
                    cnt++;
            }

            if (tmp_ot == SETS_OP_INTERSECTION && cnt == (int)vec_idex.size())
                res.insert(ii->first);  
            else if (tmp_ot == SETS_OP_UNION && cnt)
                res.insert(ii->first);  
        }
    }
    else
    {
        for (auto k = search.begin(); k != search.end(); k++)
        {
            auto ii = search_index->id_single.find(*k);
            if (ii == search_index->id_single.end())
                continue;

            const std::deque< std::shared_ptr<single_vec>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++)
            {
                if (len  < 1 + abs(vec_idex[i].date_index)) 
                    continue;

                if (tt[len - abs(vec_idex[i].date_index) - 1]->size() <= vec_idex[i].single_index)
                    continue;

                if (tt[len - abs(vec_idex[i].date_index) - 1]->at(vec_idex[i].single_index).diff >= vec_idex[i].single)
                    cnt++;
            }

            if (tmp_ot == SETS_OP_INTERSECTION && cnt == (int)vec_idex.size())
                res.insert(ii->first);  
            else if (tmp_ot == SETS_OP_UNION && cnt)
                res.insert(ii->first);  
        }
    }

    res = search;

    return true;
}

bool hsingle_search_index::do_hsingle_diff_ge_num_ge(std::string &key, std::string &value, std::set<std::string> & search)
{
    std::vector<hidex_item> vec_idex;
    hsingle_search_item * search_index = current();
    SETS_OP_TRPE tmp_ot;
    proc_data* p_data = proc_data::instance();
    strategy_conf * strategy = p_data->_conf->_strategy->current(); 

    std::vector<std::string> tmp_vec;

    if (strstr(value.c_str(), "|")) 
    {
        SplitString(value.c_str(), '|', &tmp_vec, SPLIT_MODE_ALL);
        tmp_ot = SETS_OP_UNION;
    }
    else 
    {
        SplitString(value.c_str(), ';', &tmp_vec, SPLIT_MODE_ALL); 
        tmp_ot = SETS_OP_INTERSECTION;
    }

    if (!tmp_vec.size())
        tmp_vec.push_back(value);

    //-1:0:1:2:10000&-2:0:2:1:50000
    
    for (uint32_t i = 0; i< tmp_vec.size(); i++)
    {
        std::vector<std::string> t_vec;
        SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 3)
            continue;

        hidex_item hi;
        hi.date_index = atoi(t_vec[0].c_str());
        hi.date_index_end = atoi(t_vec[1].c_str());
        hi.single_index = atoi(t_vec[2].c_str());
        hi.sum = atoi(t_vec[3].c_str());
        hi.single = atoi(t_vec[4].c_str());
        vec_idex.push_back(hi);

        if (hi.single_index >= strategy->real_single_scale.size()) 
            return false;

        if (abs(hi.date_index_end) > abs(hi.date_index))
            return false;

    }

    int cnt = 0;
    int count = 0;
    std::set<std::string> res;
    if (search.empty()) 
    {
        for (auto ii = search_index->id_single.begin(); ii != search_index->id_single.end(); ii++)
        {
            const std::deque< std::shared_ptr<single_vec>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++)
            {
                if (len  < 1 + abs(vec_idex[i].date_index)) 
                    continue;

                if (tt[len - abs(vec_idex[i].date_index) - 1]->size() <= vec_idex[i].single_index)
                    continue;

                count = 0;        
                for (int k = len - abs(vec_idex[i].date_index) - 1; k <= len - abs(vec_idex[i].date_index_end) - 1; k++) {
                    if (tt[k]->at(vec_idex[i].single_index).diff >= vec_idex[i].single)
                        count++;
                }
                if (count >= vec_idex[i].sum)
                    cnt++;
            }

            if (tmp_ot == SETS_OP_INTERSECTION && cnt == (int)vec_idex.size())
                res.insert(ii->first);  
            else if (tmp_ot == SETS_OP_UNION && cnt)
                res.insert(ii->first);  
        }
    }
    else
    {
        for (auto k = search.begin(); k != search.end(); k++)
        {
            auto ii = search_index->id_single.find(*k);
            if (ii == search_index->id_single.end())
                continue;

            const std::deque< std::shared_ptr<single_vec>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++)
            {
                if (len  < 1 + abs(vec_idex[i].date_index)) 
                    continue;

                if (tt[len - abs(vec_idex[i].date_index) - 1]->size() <= vec_idex[i].single_index)
                    continue;

                count = 0;        
                for (int k = len - abs(vec_idex[i].date_index) - 1; k <= len - abs(vec_idex[i].date_index_end) - 1; k++) {
                    if (tt[k]->at(vec_idex[i].single_index).diff >= vec_idex[i].single)
                        count++;
                }
                if (count >= vec_idex[i].sum)
                    cnt++;
            }

            if (tmp_ot == SETS_OP_INTERSECTION && cnt == (int)vec_idex.size())
                res.insert(ii->first);  
            else if (tmp_ot == SETS_OP_UNION && cnt)
                res.insert(ii->first);  
        }
    }

    res = search;

    return true;
}

bool hsingle_search_index::do_hsingle_sum_ge(std::string &key, std::string &value, std::set<std::string> & search)
{
    std::vector<hidex_item> vec_idex;
    hsingle_search_item * search_index = current();
    SETS_OP_TRPE tmp_ot;
    proc_data* p_data = proc_data::instance();
    strategy_conf * strategy = p_data->_conf->_strategy->current(); 

    std::vector<std::string> tmp_vec;

    if (strstr(value.c_str(), "|")) 
    {
        SplitString(value.c_str(), '|', &tmp_vec, SPLIT_MODE_ALL);
        tmp_ot = SETS_OP_UNION;
    }
    else 
    {
        SplitString(value.c_str(), ';', &tmp_vec, SPLIT_MODE_ALL); 
        tmp_ot = SETS_OP_INTERSECTION;
    }

    if (!tmp_vec.size())
        tmp_vec.push_back(value);

    //-1:0:1:10000&-2:0:2:50000
    
    for (uint32_t i = 0; i< tmp_vec.size(); i++)
    {
        std::vector<std::string> t_vec;
        SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 3)
            continue;

        hidex_item hi;
        hi.date_index = atoi(t_vec[0].c_str());
        hi.date_index_end = atoi(t_vec[1].c_str());
        hi.single_index = atoi(t_vec[2].c_str());
        hi.single = atoi(t_vec[3].c_str());
        vec_idex.push_back(hi);

        if (hi.single_index >= strategy->real_single_scale.size()) 
            return false;

        if (abs(hi.date_index_end) > abs(hi.date_index))
            return false;

    }

    int cnt = 0;
    int count = 0;
    std::set<std::string> res;
    if (search.empty())
    {
        for (auto ii = search_index->id_sum_single.begin(); ii != search_index->id_sum_single.end(); ii++)
        {
            const std::deque< std::shared_ptr<single_vec>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++)
            {
                if (len  < 1 + abs(vec_idex[i].date_index)) 
                    continue;

                if (tt[len - abs(vec_idex[i].date_index) - 1]->size() <= vec_idex[i].single_index)
                    continue;

                count = 0;        
                
                int k = len - abs(vec_idex[i].date_index) - 2;
                if (k < 0)
                    count = tt[len - abs(vec_idex[i].date_index_end) -1]->at(vec_idex[i].single_index).diff;
                else
                    count = tt[len - abs(vec_idex[i].date_index_end) -1]->at(vec_idex[i].single_index).diff - tt[k]->at(vec_idex[i].single_index).diff;

                if (count >= vec_idex[i].single)
                    cnt++;
            }

            if (tmp_ot == SETS_OP_INTERSECTION && cnt == (int)vec_idex.size())
                res.insert(ii->first);  
            else if (tmp_ot == SETS_OP_UNION && cnt)
                res.insert(ii->first);  
        }
    }
    else
    {
        for (auto k = search.begin(); k != search.end(); k++)
        {
            auto ii = search_index->id_sum_single.find(*k);
            if (ii == search_index->id_sum_single.end())
                continue;

            const std::deque< std::shared_ptr<single_vec>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++)
            {
                if (len  < 1 + abs(vec_idex[i].date_index)) 
                    continue;

                if (tt[len - abs(vec_idex[i].date_index) - 1]->size() <= vec_idex[i].single_index)
                    continue;

                count = 0;        

                int k = len - abs(vec_idex[i].date_index) - 2;
                if (k < 0)
                    count = tt[len - abs(vec_idex[i].date_index_end) -1]->at(vec_idex[i].single_index).diff;
                else
                    count = tt[len - abs(vec_idex[i].date_index_end) -1]->at(vec_idex[i].single_index).diff - tt[k]->at(vec_idex[i].single_index).diff;
                
                if (count >= vec_idex[i].single)
                    cnt++;
            }

            if (tmp_ot == SETS_OP_INTERSECTION && cnt == (int)vec_idex.size())
                res.insert(ii->first);  
            else if (tmp_ot == SETS_OP_UNION && cnt)
                res.insert(ii->first);  
        }
    }

    res = search;

    return true;
}

bool hsingle_search_index::do_hsingle_sum_le(std::string &key, std::string &value, std::set<std::string> & search)
{
    std::vector<hidex_item> vec_idex;
    hsingle_search_item * search_index = current();
    SETS_OP_TRPE tmp_ot;
    proc_data* p_data = proc_data::instance();
    strategy_conf * strategy = p_data->_conf->_strategy->current(); 

    std::vector<std::string> tmp_vec;

    if (strstr(value.c_str(), "|")) 
    {
        SplitString(value.c_str(), '|', &tmp_vec, SPLIT_MODE_ALL);
        tmp_ot = SETS_OP_UNION;
    }
    else 
    {
        SplitString(value.c_str(), ';', &tmp_vec, SPLIT_MODE_ALL); 
        tmp_ot = SETS_OP_INTERSECTION;
    }

    if (!tmp_vec.size())
        tmp_vec.push_back(value);

    //-1:0:1:10000&-2:0:2:50000
    
    for (uint32_t i = 0; i< tmp_vec.size(); i++)
    {
        std::vector<std::string> t_vec;
        SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 3)
            continue;

        hidex_item hi;
        hi.date_index = atoi(t_vec[0].c_str());
        hi.date_index_end = atoi(t_vec[1].c_str());
        hi.single_index = atoi(t_vec[2].c_str());
        hi.single = atoi(t_vec[3].c_str());
        vec_idex.push_back(hi);

        if (hi.single_index >= strategy->real_single_scale.size()) 
            return false;

        if (abs(hi.date_index_end) > abs(vec_idex[i].date_index))
            return false;

    }

    int cnt = 0;
    int count = 0;
    std::set<std::string> res;
    if (search.empty())
    {
        for (auto ii = search_index->id_sum_single.begin(); ii != search_index->id_sum_single.end(); ii++)
        {
            const std::deque< std::shared_ptr<single_vec>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++)
            {
                if (len  < 1 + abs(vec_idex[i].date_index)) 
                    continue;

                if (tt[len - abs(vec_idex[i].date_index) - 1]->size() <= vec_idex[i].single_index)
                    continue;

                count = 0;        

                int k = len - abs(vec_idex[i].date_index) - 2;
                if (k < 0)
                    count = tt[len - abs(vec_idex[i].date_index_end) -1]->at(vec_idex[i].single_index).diff;
                else
                    count = tt[len - abs(vec_idex[i].date_index_end) -1]->at(vec_idex[i].single_index).diff - tt[k]->at(vec_idex[i].single_index).diff;

                if (count <= vec_idex[i].single)
                    cnt++;
            }

            if (tmp_ot == SETS_OP_INTERSECTION && cnt == (int)vec_idex.size())
                res.insert(ii->first);  
            else if (tmp_ot == SETS_OP_UNION && cnt)
                res.insert(ii->first);  
        }
    }
    else
    {
        for (auto k = search.begin(); k != search.end(); k++)
        {
            auto ii = search_index->id_sum_single.find(*k);
            if (ii == search_index->id_sum_single.end())
                continue;

            const std::deque< std::shared_ptr<single_vec>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++)
            {
                if (len  < 1 + abs(vec_idex[i].date_index)) 
                    continue;

                if (tt[len - abs(vec_idex[i].date_index) - 1]->size() <= vec_idex[i].single_index)
                    continue;

                count = 0;        

                int k = len - abs(vec_idex[i].date_index) - 2;
                if (k < 0)
                    count = tt[len - abs(vec_idex[i].date_index_end) -1]->at(vec_idex[i].single_index).diff;
                else
                    count = tt[len - abs(vec_idex[i].date_index_end) -1]->at(vec_idex[i].single_index).diff - tt[k]->at(vec_idex[i].single_index).diff;

                if (count <= vec_idex[i].single)
                    cnt++;
            }

            if (tmp_ot == SETS_OP_INTERSECTION && cnt == (int)vec_idex.size())
                res.insert(ii->first);  
            else if (tmp_ot == SETS_OP_UNION && cnt)
                res.insert(ii->first);  
        }
    }

    res = search;

    return true;
}


