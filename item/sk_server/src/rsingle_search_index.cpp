#include "rsingle_search_index.h"
#include "proc_data.h"

bool rsingle_search_index::do_check_rsingle_le(std::string &name, std::string &value, search_res & search)
{
    uint32_t index = 0;
    int end = 0;
    std::multimap<int, std::string>::iterator it_le, it_ge, it;

    auto & search_index = current()->rsingle_diff_index;

    //2:50000 

    std::vector<std::string> tmp_vec;
    SplitString(value.c_str(), ':', &tmp_vec, SPLIT_MODE_ALL); 
    if (tmp_vec.size() < 2)
        return false;

    index = atoi(tmp_vec[0].c_str());

    if (index >= search_index.size()) 
        return false;

    it_le = search_index[index].end();
    it_ge = search_index[index].begin();

    end = atoi(tmp_vec[1].c_str());

    it_le = search_index[index].upper_bound(end);

    for (it = it_ge; it != it_le; ++it)
    {
        if (search.empty()|| search._id_sets.count(it->second))
        {
            search.append(name, it->second);
        }
    }

    return true;
}

bool rsingle_search_index::do_check_rsingle_ge(std::string &name, std::string &value, search_res & search)
{
    uint32_t index = 0;
    int end = 0;
    std::multimap<int, std::string>::iterator it_le, it_ge, it;

    auto & search_index = current()->rsingle_diff_index;

    //2:50000 

    std::vector<std::string> tmp_vec;
    SplitString(value.c_str(), ':', &tmp_vec, SPLIT_MODE_ALL); 
    if (tmp_vec.size() < 2)
        return false;

    index = atoi(tmp_vec[0].c_str());

    if (index >= search_index.size()) 
        return false;

    it_le = search_index[index].end();
    it_ge = search_index[index].begin();

    end = atoi(tmp_vec[1].c_str());

    it_ge = search_index[index].lower_bound(end);

    for (it = it_ge; it != it_le; ++it)
    {
        if (search.empty()|| search._id_sets.count(it->second))
        {
            search.append(name, it->second);
        }
    }

    return true;
}

bool rsingle_search_index::do_check_rsingle_diff2_le(std::string &name, std::string &value, search_res & search)
{
    std::vector<hidex_item> vec_idex;
    auto * search_index = current();
    proc_data* p_data = proc_data::instance();
    strategy_conf * strategy = p_data->_conf->_strategy->current(); 

    std::vector<std::string> tmp_vec;


        tmp_vec.push_back(value);

    //-1:-2:1:10000&-2:-3:2:50000 

    for (uint32_t i = 0; i< tmp_vec.size(); i++)
    {
        std::vector<std::string> t_vec;
        SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 4)
            continue;

        hidex_item hi;
        hi.date_index = atoi(t_vec[0].c_str());
        hi.date_index_end = atoi(t_vec[1].c_str());
        hi.single_index = atoi(t_vec[2].c_str());
        hi.single = atoi(t_vec[3].c_str());
        vec_idex.push_back(hi);

        if (hi.single_index >= strategy->real_single_scale.size()) 
            return false;
    }

    int cnt = 0;
    if (search.empty()) 
    {
        for (auto ii = search_index->id_single.begin(); ii != search_index->id_single.end(); ii++)
        {
            const std::deque< std::shared_ptr<single_vec>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++)
            {
                if (len  < 1 + abs(vec_idex[i].date_index) || len  < 1 + abs(vec_idex[i].date_index_end)) 
                    continue;

                if (tt[len - abs(vec_idex[i].date_index) - 1]->size() <= vec_idex[i].single_index)
                    continue;

                if (tt[len - abs(vec_idex[i].date_index_end) - 1]->size() <= vec_idex[i].single_index)
                    continue;

                int sdiff = tt[len - abs(vec_idex[i].date_index) - 1]->at(vec_idex[i].single_index).diff;
                int ediff = tt[len - abs(vec_idex[i].date_index_end) - 1]->at(vec_idex[i].single_index).diff;

                if (sdiff <= vec_idex[i].single + ediff)
                    cnt++;
            }

            if (cnt)
                search.append(name, ii->first);
        }
    }
    else
    {
        for (auto k = search._id_sets.begin(); k != search._id_sets.end(); k++)
        {
            auto ii = search_index->id_single.find(*k);
            if (ii == search_index->id_single.end())
                continue;

            const std::deque< std::shared_ptr<single_vec>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++)
            {
                if (len  < 1 + abs(vec_idex[i].date_index) || len  < 1 + abs(vec_idex[i].date_index_end)) 
                    continue;

                if (tt[len - abs(vec_idex[i].date_index) - 1]->size() <= vec_idex[i].single_index)
                    continue;

                if (tt[len - abs(vec_idex[i].date_index_end) - 1]->size() <= vec_idex[i].single_index)
                    continue;

                int sdiff = tt[len - abs(vec_idex[i].date_index) - 1]->at(vec_idex[i].single_index).diff;
                int ediff = tt[len - abs(vec_idex[i].date_index_end) - 1]->at(vec_idex[i].single_index).diff;

                if (sdiff <= vec_idex[i].single + ediff)
                    cnt++;
            }

            if (cnt)
                search.append(name, ii->first);
        }
    }


    return true;
}

bool rsingle_search_index::do_check_rsingle_diff2_ge(std::string &name, std::string &value, search_res & search)
{
    std::vector<hidex_item> vec_idex;
    auto * search_index = current();
    proc_data* p_data = proc_data::instance();
    strategy_conf * strategy = p_data->_conf->_strategy->current(); 

    std::vector<std::string> tmp_vec;


        tmp_vec.push_back(value);

    //-1:-2:1:10000&-2:-3:2:50000 

    for (uint32_t i = 0; i< tmp_vec.size(); i++)
    {
        std::vector<std::string> t_vec;
        SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 4)
            continue;

        hidex_item hi;
        hi.date_index = atoi(t_vec[0].c_str());
        hi.date_index_end = atoi(t_vec[1].c_str());
        hi.single_index = atoi(t_vec[2].c_str());
        hi.single = atoi(t_vec[3].c_str());
        vec_idex.push_back(hi);

        if (hi.single_index >= strategy->real_single_scale.size()) 
            return false;
    }

    int cnt = 0;
    if (search.empty()) 
    {
        for (auto ii = search_index->id_single.begin(); ii != search_index->id_single.end(); ii++)
        {
            const std::deque< std::shared_ptr<single_vec>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++)
            {
                if (len  < 1 + abs(vec_idex[i].date_index) || len  < 1 + abs(vec_idex[i].date_index_end)) 
                    continue;

                if (tt[len - abs(vec_idex[i].date_index) - 1]->size() <= vec_idex[i].single_index)
                    continue;

                if (tt[len - abs(vec_idex[i].date_index_end) - 1]->size() <= vec_idex[i].single_index)
                    continue;

                int sdiff = tt[len - abs(vec_idex[i].date_index) - 1]->at(vec_idex[i].single_index).diff;
                int ediff = tt[len - abs(vec_idex[i].date_index_end) - 1]->at(vec_idex[i].single_index).diff;

                if (sdiff >= vec_idex[i].single + ediff)
                    cnt++;
            }

            if (cnt)
                search.append(name, ii->first);
        }
    }
    else
    {
        for (auto k = search._id_sets.begin(); k != search._id_sets.end(); k++)
        {
            auto ii = search_index->id_single.find(*k);
            if (ii == search_index->id_single.end())
                continue;

            const std::deque< std::shared_ptr<single_vec>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++)
            {
                if (len  < 1 + abs(vec_idex[i].date_index) || len  < 1 + abs(vec_idex[i].date_index_end)) 
                    continue;

                if (tt[len - abs(vec_idex[i].date_index) - 1]->size() <= vec_idex[i].single_index)
                    continue;

                if (tt[len - abs(vec_idex[i].date_index_end) - 1]->size() <= vec_idex[i].single_index)
                    continue;

                int sdiff = tt[len - abs(vec_idex[i].date_index) - 1]->at(vec_idex[i].single_index).diff;
                int ediff = tt[len - abs(vec_idex[i].date_index_end) - 1]->at(vec_idex[i].single_index).diff;

                if (sdiff >= vec_idex[i].single + ediff)
                    cnt++;
            }

            if (cnt)
                search.append(name, ii->first);
        }
    }


    return true;

}


