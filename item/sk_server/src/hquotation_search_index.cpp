#include "hquotation_search_index.h"
#include "sk_util.h"
#include "proc_data.h"
#include "history_quotation_dict.h"

bool hquotation_search_index::do_hqchange_rate_le(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    std::vector<std::string> tmp_vec;
    std::vector<hidex_item> vec_idex;


    tmp_vec.push_back(value);

    //-1:1.0&-2:1
    for (uint32_t i = 0; i< tmp_vec.size(); i++)
    {
        std::vector<std::string> t_vec;
        SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 2) 
        { 
            continue;
        }

        hidex_item hi;
        hi.date_index = atoi(t_vec[0].c_str());
        hi.fpoint = atof(t_vec[1].c_str());
        vec_idex.push_back(hi);
    }

    int cnt = 0;
    if (search.empty())
    {
        for (auto ii = search_index->id_quotation.begin(); ii != search_index->id_quotation.end(); ii++)
        {
            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                if (tt[len - abs(vec_idex[i].date_index) - 1]->change_rate <= vec_idex[i].fpoint)
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
            auto ii = search_index->id_quotation.find(*k);
            if (ii == search_index->id_quotation.end())
                continue;

            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                if (tt[len - abs(vec_idex[i].date_index) - 1]->change_rate <= vec_idex[i].fpoint)
                    cnt++;
            }


            if (cnt)
                search.append(name, ii->first);
        }
    }


    return true;
}

bool hquotation_search_index::do_hqchange_rate_ge(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    std::vector<std::string> tmp_vec;
    std::vector<hidex_item> vec_idex;

    tmp_vec.push_back(value);

    //-1:1.0&-2:1
    for (uint32_t i = 0; i< tmp_vec.size(); i++)
    {
        std::vector<std::string> t_vec;
        SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 2) 
        { 
            continue;
        }

        hidex_item hi;
        hi.date_index = atoi(t_vec[0].c_str());
        hi.fpoint = atof(t_vec[1].c_str());
        vec_idex.push_back(hi);
    }

    int cnt = 0;

    if (search.empty())
    {
        for (auto ii = search_index->id_quotation.begin(); ii != search_index->id_quotation.end(); ii++)
        {
            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                if (tt[len - abs(vec_idex[i].date_index) - 1]->change_rate >= vec_idex[i].fpoint)
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
            auto ii = search_index->id_quotation.find(*k);
            if (ii == search_index->id_quotation.end())
                continue;

            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                if (tt[len - abs(vec_idex[i].date_index) - 1]->change_rate >= vec_idex[i].fpoint)
                    cnt++;
            }

            if (cnt)
                search.append(name, ii->first);
        }
    }



    return true;
}

bool hquotation_search_index::do_hqrange_percent_le(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    std::vector<std::string> tmp_vec;
    std::vector<hidex_item> vec_idex;


    tmp_vec.push_back(value);

    //-1:1.0&-2:1
    for (uint32_t i = 0; i< tmp_vec.size(); i++)
    {
        std::vector<std::string> t_vec;
        SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 2) 
        { 
            continue;
        }

        hidex_item hi;
        hi.date_index = atoi(t_vec[0].c_str());
        hi.fpoint = atof(t_vec[1].c_str());
        vec_idex.push_back(hi);
    }

    int cnt = 0;

    if (search.empty())
    {
        for (auto ii = search_index->id_quotation.begin(); ii != search_index->id_quotation.end(); ii++)
        {
            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                if (tt[len - abs(vec_idex[i].date_index) - 1]->range_percent <= vec_idex[i].fpoint)
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
            auto ii = search_index->id_quotation.find(*k);
            if (ii == search_index->id_quotation.end())
                continue;

            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                if (tt[len - abs(vec_idex[i].date_index) - 1]->range_percent <= vec_idex[i].fpoint)
                    cnt++;
            }

            if (cnt)
                search.append(name, ii->first);
        }
    }



    return true;
}

bool hquotation_search_index::do_hqrange_percent_ge(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    std::vector<std::string> tmp_vec;
    std::vector<hidex_item> vec_idex;


    tmp_vec.push_back(value);

    //-1:1.0&-2:1
    for (uint32_t i = 0; i< tmp_vec.size(); i++)
    {
        std::vector<std::string> t_vec;
        SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 2) 
        { 
            continue;
        }

        hidex_item hi;
        hi.date_index = atoi(t_vec[0].c_str());
        hi.fpoint = atof(t_vec[1].c_str());
        vec_idex.push_back(hi);
    }

    int cnt = 0;

    if (search.empty())
    {
        for (auto ii = search_index->id_quotation.begin(); ii != search_index->id_quotation.end(); ii++)
        {
            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                if (tt[len - abs(vec_idex[i].date_index) - 1]->range_percent >= vec_idex[i].fpoint)
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
            auto ii = search_index->id_quotation.find(*k);
            if (ii == search_index->id_quotation.end())
                continue;

            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                if (tt[len - abs(vec_idex[i].date_index) - 1]->range_percent >= vec_idex[i].fpoint)
                    cnt++;
            }

            if (cnt)
                search.append(name, ii->first);
        }
    }



    return true;
}

bool hquotation_search_index::do_hqrange_percent_ge_num_ge(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    std::vector<std::string> tmp_vec;
    std::vector<hidex_item> vec_idex;


    tmp_vec.push_back(value);

    //-5:0:2:1.0&-2:0:1:1
    for (uint32_t i = 0; i< tmp_vec.size(); i++)
    {
        std::vector<std::string> t_vec;
        SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 4) 
        { 
            continue;
        }

        hidex_item hi;
        hi.date_index = atoi(t_vec[0].c_str());
        hi.date_index_end = atoi(t_vec[1].c_str());
        hi.sum = atoi(t_vec[2].c_str());
        hi.fpoint = atof(t_vec[3].c_str());

        if (abs(hi.date_index) < abs(hi.date_index_end))
            continue;

        vec_idex.push_back(hi);
    }

    int cnt = 0;
    int count = 0;

    if (search.empty())
    {
        for (auto ii = search_index->id_quotation.begin(); ii != search_index->id_quotation.end(); ii++)
        {
            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                count = 0;               
                for (int k = len - abs(vec_idex[i].date_index) - 1; k <= len - abs(vec_idex[i].date_index_end) - 1; k++) {
                    if (tt[k]->range_percent >= vec_idex[i].fpoint)
                        count++;
                }

                if (count >= vec_idex[i].sum)
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
            auto ii = search_index->id_quotation.find(*k);
            if (ii == search_index->id_quotation.end())
                continue;

            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                count = 0;               
                for (int k = len - abs(vec_idex[i].date_index) - 1; k <= len - abs(vec_idex[i].date_index_end) - 1; k++) {
                    if (tt[k]->range_percent >= vec_idex[i].fpoint)
                        count++;
                }


                if (count >= vec_idex[i].sum)
                    cnt++;
            }

            if (cnt)
                search.append(name, ii->first);
        }
    }



    return true;
}

bool hquotation_search_index::do_hqrange_percent_le_num_ge(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    std::vector<std::string> tmp_vec;
    std::vector<hidex_item> vec_idex;


    tmp_vec.push_back(value);

    //-5:0:2:1.0&-2:0:1:1
    for (uint32_t i = 0; i< tmp_vec.size(); i++)
    {
        std::vector<std::string> t_vec;
        SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 4) 
        { 
            continue;
        }

        hidex_item hi;
        hi.date_index = atoi(t_vec[0].c_str());
        hi.date_index_end = atoi(t_vec[1].c_str());
        hi.sum = atoi(t_vec[2].c_str());
        hi.fpoint = atof(t_vec[3].c_str());

        if (abs(hi.date_index) < abs(hi.date_index_end))
            continue;

        vec_idex.push_back(hi);
    }

    int cnt = 0;
    int count = 0;

    if (search.empty())
    {
        for (auto ii = search_index->id_quotation.begin(); ii != search_index->id_quotation.end(); ii++)
        {
            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                count = 0;               
                for (int k = len - abs(vec_idex[i].date_index) - 1; k <= len - abs(vec_idex[i].date_index_end) - 1; k++) {
                    if (tt[k]->range_percent <= vec_idex[i].fpoint)
                        count++;
                }

                if (count >= vec_idex[i].sum)
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
            auto ii = search_index->id_quotation.find(*k);
            if (ii == search_index->id_quotation.end())
                continue;

            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                count = 0;               
                for (int k = len - abs(vec_idex[i].date_index) - 1; k <= len - abs(vec_idex[i].date_index_end) - 1; k++) {
                    if (tt[k]->range_percent <= vec_idex[i].fpoint)
                        count++;
                }


                if (count >= vec_idex[i].sum)
                    cnt++;
            }

            if (cnt)
                search.append(name, ii->first);
        }
    }



    return true;
}

bool hquotation_search_index::do_hq_sum_change_rate_le(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    std::vector<std::string> tmp_vec;
    std::vector<hidex_item> vec_idex;


    tmp_vec.push_back(value);

    //-1:0:1.0&-2:0:1
    for (uint32_t i = 0; i< tmp_vec.size(); i++)
    {
        std::vector<std::string> t_vec;
        SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 3) 
        { 
            continue;
        }

        hidex_item hi;
        hi.date_index = atoi(t_vec[0].c_str());
        hi.date_index_end = atoi(t_vec[1].c_str());

        if (abs(hi.date_index) < abs(hi.date_index_end))
            continue;

        hi.fpoint = atof(t_vec[2].c_str());
        vec_idex.push_back(hi);
    }

    int cnt = 0;
    float count = 0;

    if (search.empty())
    {
        for (auto ii = search_index->id_sum_quotation.begin(); ii != search_index->id_sum_quotation.end(); ii++)
        {
            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 2 + abs(vec_idex[i].date_index))
                    continue;

                count = 0;
                int k = len - abs(vec_idex[i].date_index) - 2;
                count = tt[len - abs(vec_idex[i].date_index_end) -1]->change_rate - tt[k]->change_rate;

                if (count <= vec_idex[i].fpoint)
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
            auto ii = search_index->id_sum_quotation.find(*k);
            if (ii == search_index->id_sum_quotation.end())
                continue;

            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 2 + abs(vec_idex[i].date_index))
                    continue;

                count = 0;

                int k = len - abs(vec_idex[i].date_index) - 2;
                count = tt[len - abs(vec_idex[i].date_index_end) -1]->change_rate - tt[k]->change_rate;

                if (count <= vec_idex[i].fpoint)
                    cnt++;
            }

            if (cnt)
                search.append(name, ii->first);
        }
    }



    return true;
}

bool hquotation_search_index::do_hq_sum_change_rate_ge(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    std::vector<std::string> tmp_vec;
    std::vector<hidex_item> vec_idex;


    tmp_vec.push_back(value);

    //-1:0:1.0&-2:0:1
    for (uint32_t i = 0; i< tmp_vec.size(); i++)
    {
        std::vector<std::string> t_vec;
        SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 3) 
        { 
            continue;
        }

        hidex_item hi;
        hi.date_index = atoi(t_vec[0].c_str());
        hi.date_index_end = atoi(t_vec[1].c_str());

        if (abs(hi.date_index) < abs(hi.date_index_end))
            continue;

        hi.fpoint = atof(t_vec[2].c_str());
        vec_idex.push_back(hi);
    }

    int cnt = 0;
    float count = 0;

    if (search.empty())
    {
        for (auto ii = search_index->id_sum_quotation.begin(); ii != search_index->id_sum_quotation.end(); ii++)
        {
            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 2 + abs(vec_idex[i].date_index))
                    continue;

                count = 0;

                int k = len - abs(vec_idex[i].date_index) - 2;
                count = tt[len - abs(vec_idex[i].date_index_end) -1]->change_rate - tt[k]->change_rate;

                if (count >= vec_idex[i].fpoint)
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
            auto ii = search_index->id_sum_quotation.find(*k);
            if (ii == search_index->id_sum_quotation.end())
                continue;

            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 2 + abs(vec_idex[i].date_index))
                    continue;

                count = 0;

                int k = len - abs(vec_idex[i].date_index) - 2;
                count = tt[len - abs(vec_idex[i].date_index_end) -1]->change_rate - tt[k]->change_rate;

                if (count >= vec_idex[i].fpoint)
                    cnt++;
            }

            if (cnt)
                search.append(name, ii->first);
        }
    }



    return true;
}

bool hquotation_search_index::do_hq_sum_range_percent_le(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    std::vector<std::string> tmp_vec;
    std::vector<hidex_item> vec_idex;


    tmp_vec.push_back(value);

    //-1:0:1.0&-2:0:1
    for (uint32_t i = 0; i< tmp_vec.size(); i++)
    {
        std::vector<std::string> t_vec;
        SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 3) 
        { 
            continue;
        }

        hidex_item hi;
        hi.date_index = atoi(t_vec[0].c_str());
        hi.date_index_end = atoi(t_vec[1].c_str());

        if (abs(hi.date_index) < abs(hi.date_index_end))
            continue;

        hi.fpoint = atof(t_vec[2].c_str());
        vec_idex.push_back(hi);
    }

    int cnt = 0;
    float count = 0;

    if (search.empty())
    {
        for (auto ii = search_index->id_sum_quotation.begin(); ii != search_index->id_sum_quotation.end(); ii++)
        {
            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 2 + abs(vec_idex[i].date_index))
                    continue;

                count = 0;
                int k = len - abs(vec_idex[i].date_index) - 2;
                count = tt[len - abs(vec_idex[i].date_index_end) -1]->range_percent - tt[k]->range_percent;

                if (count <= vec_idex[i].fpoint)
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
            auto ii = search_index->id_sum_quotation.find(*k);
            if (ii == search_index->id_sum_quotation.end())
                continue;

            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 2 + abs(vec_idex[i].date_index))
                    continue;

                count = 0;
                int k = len - abs(vec_idex[i].date_index) - 2;
                count = tt[len - abs(vec_idex[i].date_index_end) -1]->range_percent - tt[k]->range_percent;

                if (count <= vec_idex[i].fpoint)
                    cnt++;
            }

            if (cnt)
                search.append(name, ii->first);
        }
    }



    return true;
}


bool hquotation_search_index::do_hq_sum_range_percent_ge(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    std::vector<std::string> tmp_vec;
    std::vector<hidex_item> vec_idex;


    tmp_vec.push_back(value);

    //-1:0:1.0&-2:0:1
    for (uint32_t i = 0; i< tmp_vec.size(); i++)
    {
        std::vector<std::string> t_vec;
        SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 3) 
        { 
            continue;
        }

        hidex_item hi;
        hi.date_index = atoi(t_vec[0].c_str());
        hi.date_index_end = atoi(t_vec[1].c_str());

        if (abs(hi.date_index) < abs(hi.date_index_end))
            continue;

        hi.fpoint = atof(t_vec[2].c_str());
        vec_idex.push_back(hi);
    }

    int cnt = 0;
    float count = 0;

    if (search.empty())
    {
        for (auto ii = search_index->id_sum_quotation.begin(); ii != search_index->id_sum_quotation.end(); ii++)
        {
            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 2 + abs(vec_idex[i].date_index))
                    continue; 

                int k = len - abs(vec_idex[i].date_index) - 2;
                count = tt[len - abs(vec_idex[i].date_index_end) -1]->range_percent - tt[k]->range_percent;

                if (count >= vec_idex[i].fpoint)
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
            auto ii = search_index->id_sum_quotation.find(*k);
            if (ii == search_index->id_sum_quotation.end())
                continue;

            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 2 + abs(vec_idex[i].date_index))
                    continue;

                count = 0;
                int k = len - abs(vec_idex[i].date_index) - 2;
                count = tt[len - abs(vec_idex[i].date_index_end) -1]->range_percent - tt[k]->range_percent;

                if (count >= vec_idex[i].fpoint)
                    cnt++;
            }

            if (cnt)
                search.append(name, ii->first);
        }
    }



    return true;
}

bool hquotation_search_index::do_check_hqdown_pointer_ge(std::string &name, std::string &value, search_res & search)                                              
{
    hquotation_search_item * search_index = current();
    SETS_OP_TRPE tmp_ot;
    proc_data* p_data = proc_data::instance();

    std::vector<std::string> tmp_vec;
    std::vector<hidex_item> vec_idex;


    tmp_vec.push_back(value);

    //-1:1.0&-2:1
    for (uint32_t i = 0; i< tmp_vec.size(); i++)
    {
        std::vector<std::string> t_vec;
        SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 2) 
        { 
            continue;
        }

        hidex_item hi;
        hi.date_index = atoi(t_vec[0].c_str());
        hi.fpoint = atof(t_vec[1].c_str());
        vec_idex.push_back(hi);
    }

    int cnt = 0;

    if (search.empty())
    {
        for (auto ii = search_index->id_technical.begin(); ii != search_index->id_technical.end(); ii++)
        {
            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                if (tt[len - abs(vec_idex[i].date_index) - 1]->down_pointer >= vec_idex[i].fpoint)
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
            auto ii = search_index->id_technical.find(*k);
            if (ii == search_index->id_technical.end())
                continue;

            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                if (tt[len - abs(vec_idex[i].date_index) - 1]->down_pointer >= vec_idex[i].fpoint)
                    cnt++;
            }

            if (cnt)
                search.append(name, ii->first);
        }
    }



    return true;
}

bool hquotation_search_index::do_check_hqdown_pointer_le(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    std::vector<std::string> tmp_vec;
    std::vector<hidex_item> vec_idex;


    tmp_vec.push_back(value);

    //-1:1.0&-2:1
    for (uint32_t i = 0; i< tmp_vec.size(); i++)
    {
        std::vector<std::string> t_vec;
        SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 2) 
        { 
            continue;
        }

        hidex_item hi;
        hi.date_index = atoi(t_vec[0].c_str());
        hi.fpoint = atof(t_vec[1].c_str());
        vec_idex.push_back(hi);
    }

    int cnt = 0;

    if (search.empty())
    {
        for (auto ii = search_index->id_technical.begin(); ii != search_index->id_technical.end(); ii++)
        {
            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                if (tt[len - abs(vec_idex[i].date_index) - 1]->down_pointer <= vec_idex[i].fpoint)
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
            auto ii = search_index->id_technical.find(*k);
            if (ii == search_index->id_technical.end())
                continue;

            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                if (tt[len - abs(vec_idex[i].date_index) - 1]->down_pointer <= vec_idex[i].fpoint)
                    cnt++;
            }

            if (cnt)
                search.append(name, ii->first);
        }
    }



    return true;
}

bool hquotation_search_index::do_check_hqdown_pointer_ge_num_ge(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    std::vector<std::string> tmp_vec;
    std::vector<hidex_item> vec_idex;


    tmp_vec.push_back(value);

    //-5:0:2:1.0&-2:0:1:1
    for (uint32_t i = 0; i< tmp_vec.size(); i++)
    {
        std::vector<std::string> t_vec;
        SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 4) 
        { 
            continue;
        }

        hidex_item hi;
        hi.date_index = atoi(t_vec[0].c_str());
        hi.date_index_end = atoi(t_vec[1].c_str());
        hi.sum = atoi(t_vec[2].c_str());
        hi.fpoint = atof(t_vec[3].c_str());

        if (abs(hi.date_index) < abs(hi.date_index_end))
            continue;

        vec_idex.push_back(hi);
    }

    int cnt = 0;
    int count = 0;

    if (search.empty())
    {
        for (auto ii = search_index->id_technical.begin(); ii != search_index->id_technical.end(); ii++)
        {
            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                count = 0;               
                for (int k = len - abs(vec_idex[i].date_index) - 1; k <= len - abs(vec_idex[i].date_index_end) - 1; k++) {
                    if (tt[k]->down_pointer >= vec_idex[i].fpoint)
                        count++;
                }

                if (count >= vec_idex[i].sum)
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
            auto ii = search_index->id_technical.find(*k);
            if (ii == search_index->id_technical.end())
                continue;

            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                count = 0;               
                for (int k = len - abs(vec_idex[i].date_index) - 1; k <= len - abs(vec_idex[i].date_index_end) - 1; k++) {
                    if (tt[k]->down_pointer >= vec_idex[i].fpoint)
                        count++;
                }


                if (count >= vec_idex[i].sum)
                    cnt++;
            }

            if (cnt)
                search.append(name, ii->first);
        }
    }



    return true;
}


bool hquotation_search_index::do_check_hqup_pointer_ge(std::string &name, std::string &value, search_res & search)                                              
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    std::vector<std::string> tmp_vec;
    std::vector<hidex_item> vec_idex;


    tmp_vec.push_back(value);

    //-1:1.0&-2:1
    for (uint32_t i = 0; i< tmp_vec.size(); i++)
    {
        std::vector<std::string> t_vec;
        SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 2) 
        { 
            continue;
        }

        hidex_item hi;
        hi.date_index = atoi(t_vec[0].c_str());
        hi.fpoint = atof(t_vec[1].c_str());
        vec_idex.push_back(hi);
    }

    int cnt = 0;

    if (search.empty())
    {
        for (auto ii = search_index->id_technical.begin(); ii != search_index->id_technical.end(); ii++)
        {
            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                if (tt[len - abs(vec_idex[i].date_index) - 1]->up_pointer >= vec_idex[i].fpoint)
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
            auto ii = search_index->id_technical.find(*k);
            if (ii == search_index->id_technical.end())
                continue;

            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                if (tt[len - abs(vec_idex[i].date_index) - 1]->up_pointer >= vec_idex[i].fpoint)
                    cnt++;
            }

            if (cnt)
                search.append(name, ii->first);
        }
    }



    return true;
}

bool hquotation_search_index::do_check_hqup_pointer_le(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    std::vector<std::string> tmp_vec;
    std::vector<hidex_item> vec_idex;


    tmp_vec.push_back(value);

    //-1:1.0&-2:1
    for (uint32_t i = 0; i< tmp_vec.size(); i++)
    {
        std::vector<std::string> t_vec;
        SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 2) 
        { 
            continue;
        }

        hidex_item hi;
        hi.date_index = atoi(t_vec[0].c_str());
        hi.fpoint = atof(t_vec[1].c_str());
        vec_idex.push_back(hi);
    }

    int cnt = 0;

    if (search.empty())
    {
        for (auto ii = search_index->id_technical.begin(); ii != search_index->id_technical.end(); ii++)
        {
            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                if (tt[len - abs(vec_idex[i].date_index) - 1]->up_pointer <= vec_idex[i].fpoint)
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
            auto ii = search_index->id_technical.find(*k);
            if (ii == search_index->id_technical.end())
                continue;

            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                if (tt[len - abs(vec_idex[i].date_index) - 1]->up_pointer <= vec_idex[i].fpoint)
                    cnt++;
            }

            if (cnt)
                search.append(name, ii->first);
        }
    }



    return true;
}

bool hquotation_search_index::do_check_hqup_pointer_ge_num_ge(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    std::vector<std::string> tmp_vec;
    std::vector<hidex_item> vec_idex;


    tmp_vec.push_back(value);

    //-5:0:2:1.0&-2:0:1:1
    for (uint32_t i = 0; i< tmp_vec.size(); i++)
    {
        std::vector<std::string> t_vec;
        SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 4) 
        { 
            continue;
        }

        hidex_item hi;
        hi.date_index = atoi(t_vec[0].c_str());
        hi.date_index_end = atoi(t_vec[1].c_str());
        hi.sum = atoi(t_vec[2].c_str());
        hi.fpoint = atof(t_vec[3].c_str());

        if (abs(hi.date_index) < abs(hi.date_index_end))
            continue;

        vec_idex.push_back(hi);
    }

    int cnt = 0;
    int count = 0;

    if (search.empty())
    {
        for (auto ii = search_index->id_technical.begin(); ii != search_index->id_technical.end(); ii++)
        {
            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                count = 0;               
                for (int k = len - abs(vec_idex[i].date_index) - 1; k <= len - abs(vec_idex[i].date_index_end) - 1; k++) {
                    if (tt[k]->up_pointer >= vec_idex[i].fpoint)
                        count++;
                }

                if (count >= vec_idex[i].sum)
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
            auto ii = search_index->id_technical.find(*k);
            if (ii == search_index->id_technical.end())
                continue;

            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                count = 0;               
                for (int k = len - abs(vec_idex[i].date_index) - 1; k <= len - abs(vec_idex[i].date_index_end) - 1; k++) {
                    if (tt[k]->up_pointer >= vec_idex[i].fpoint)
                        count++;
                }


                if (count >= vec_idex[i].sum)
                    cnt++;
            }

            if (cnt)
                search.append(name, ii->first);
        }
    }



    return true;
}

bool hquotation_search_index::do_check_hqend_start_ge(std::string &name, std::string &value, search_res & search)                                              
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    std::vector<std::string> tmp_vec;
    std::vector<hidex_item> vec_idex;


    tmp_vec.push_back(value);

    //-1:1.0&-2:1
    for (uint32_t i = 0; i< tmp_vec.size(); i++)
    {
        std::vector<std::string> t_vec;
        SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 2) 
        { 
            continue;
        }

        hidex_item hi;
        hi.date_index = atoi(t_vec[0].c_str());
        hi.fpoint = atof(t_vec[1].c_str());
        vec_idex.push_back(hi);
    }

    int cnt = 0;

    if (search.empty())
    {
        for (auto ii = search_index->id_technical.begin(); ii != search_index->id_technical.end(); ii++)
        {
            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                if (tt[len - abs(vec_idex[i].date_index) - 1]->end_start >= vec_idex[i].fpoint)
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
            auto ii = search_index->id_technical.find(*k);
            if (ii == search_index->id_technical.end())
                continue;

            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                if (tt[len - abs(vec_idex[i].date_index) - 1]->end_start >= vec_idex[i].fpoint)
                    cnt++;
            }

            if (cnt)
                search.append(name, ii->first);
        }
    }



    return true;
}

bool hquotation_search_index::do_check_hqend_start_le(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    std::vector<std::string> tmp_vec;
    std::vector<hidex_item> vec_idex;


    tmp_vec.push_back(value);

    //-1:1.0&-2:1
    for (uint32_t i = 0; i< tmp_vec.size(); i++)
    {
        std::vector<std::string> t_vec;
        SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 2) 
        { 
            continue;
        }

        hidex_item hi;
        hi.date_index = atoi(t_vec[0].c_str());
        hi.fpoint = atof(t_vec[1].c_str());
        vec_idex.push_back(hi);
    }

    int cnt = 0;

    if (search.empty())
    {
        for (auto ii = search_index->id_technical.begin(); ii != search_index->id_technical.end(); ii++)
        {
            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                if (tt[len - abs(vec_idex[i].date_index) - 1]->end_start <= vec_idex[i].fpoint)
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
            auto ii = search_index->id_technical.find(*k);
            if (ii == search_index->id_technical.end())
                continue;

            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                if (tt[len - abs(vec_idex[i].date_index) - 1]->end_start <= vec_idex[i].fpoint)
                    cnt++;
            }

            if (cnt)
                search.append(name, ii->first);
        }
    }



    return true;
}

bool hquotation_search_index::do_check_hqend_start_ge_num_ge(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    std::vector<std::string> tmp_vec;
    std::vector<hidex_item> vec_idex;


    tmp_vec.push_back(value);

    //-5:0:2:1.0&-2:0:1:1
    for (uint32_t i = 0; i< tmp_vec.size(); i++)
    {
        std::vector<std::string> t_vec;
        SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 4) 
        { 
            continue;
        }

        hidex_item hi;
        hi.date_index = atoi(t_vec[0].c_str());
        hi.date_index_end = atoi(t_vec[1].c_str());
        hi.sum = atoi(t_vec[2].c_str());
        hi.fpoint = atof(t_vec[3].c_str());

        if (abs(hi.date_index) < abs(hi.date_index_end))
            continue;

        vec_idex.push_back(hi);
    }

    int cnt = 0;
    int count = 0;

    if (search.empty())
    {
        for (auto ii = search_index->id_technical.begin(); ii != search_index->id_technical.end(); ii++)
        {
            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                count = 0;               
                for (int k = len - abs(vec_idex[i].date_index) - 1; k <= len - abs(vec_idex[i].date_index_end) - 1; k++) {
                    if (tt[k]->end_start >= vec_idex[i].fpoint)
                        count++;
                }

                if (count >= vec_idex[i].sum)
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
            auto ii = search_index->id_technical.find(*k);
            if (ii == search_index->id_technical.end())
                continue;

            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                count = 0;               
                for (int k = len - abs(vec_idex[i].date_index) - 1; k <= len - abs(vec_idex[i].date_index_end) - 1; k++) {
                    if (tt[k]->end_start >= vec_idex[i].fpoint)
                        count++;
                }


                if (count >= vec_idex[i].sum)
                    cnt++;
            }

            if (cnt)
                search.append(name, ii->first);
        }
    }



    return true;
}


bool hquotation_search_index::do_check_hqend_start_ge_ratio_ge(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    std::vector<std::string> tmp_vec;
    std::vector<hidex_item> vec_idex;


    tmp_vec.push_back(value);

    //-5:0:0.8:1.0&-2:0:0.8:1
    for (uint32_t i = 0; i< tmp_vec.size(); i++)
    {
        std::vector<std::string> t_vec;
        SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 4) 
        { 
            continue;
        }

        hidex_item hi;
        hi.date_index = atoi(t_vec[0].c_str());
        hi.date_index_end = atoi(t_vec[1].c_str());
        hi.sum = atoi(t_vec[2].c_str());
        hi.fpoint = atof(t_vec[3].c_str());

        if (abs(hi.date_index) < abs(hi.date_index_end))
            continue;

        vec_idex.push_back(hi);
    }

    int cnt = 0;
    int count = 0;

    if (search.empty())
    {
        for (auto ii = search_index->id_technical.begin(); ii != search_index->id_technical.end(); ii++)
        {
            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                count = 0;               
                for (int k = len - abs(vec_idex[i].date_index) - 1; k <= len - abs(vec_idex[i].date_index_end) - 1; k++) {
                    if (tt[k]->end_start >= vec_idex[i].fpoint)
                        count++;
                }

                if (count >= vec_idex[i].sum * (abs(vec_idex[i].date_index) -  abs(vec_idex[i].date_index_end) + 1))
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
            auto ii = search_index->id_technical.find(*k);
            if (ii == search_index->id_technical.end())
                continue;

            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                count = 0;               
                for (int k = len - abs(vec_idex[i].date_index) - 1; k <= len - abs(vec_idex[i].date_index_end) - 1; k++) {
                    if (tt[k]->end_start >= vec_idex[i].fpoint)
                        count++;
                }


                if (count >= vec_idex[i].sum * (abs(vec_idex[i].date_index) -  abs(vec_idex[i].date_index_end) + 1))
                    cnt++;
            }

            if (cnt)
                search.append(name, ii->first);
        }
    }



    return true;
}


bool hquotation_search_index::do_check_hqend_avg_end_ge(std::string &name, std::string &value, search_res & search)                                              
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    std::vector<std::string> tmp_vec;
    std::vector<hidex_item> vec_idex;


    tmp_vec.push_back(value);

    //-1:1.0&-2:1
    for (uint32_t i = 0; i< tmp_vec.size(); i++)
    {
        std::vector<std::string> t_vec;
        SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 2) 
        { 
            continue;
        }

        hidex_item hi;
        hi.date_index = atoi(t_vec[0].c_str());
        hi.fpoint = atof(t_vec[1].c_str());
        vec_idex.push_back(hi);
    }

    int cnt = 0;

    if (search.empty())
    {
        for (auto ii = search_index->id_technical.begin(); ii != search_index->id_technical.end(); ii++)
        {
            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                if (tt[len - abs(vec_idex[i].date_index) - 1]->end_avg_end >= vec_idex[i].fpoint)
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
            auto ii = search_index->id_technical.find(*k);
            if (ii == search_index->id_technical.end())
                continue;

            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                if (tt[len - abs(vec_idex[i].date_index) - 1]->end_avg_end >= vec_idex[i].fpoint)
                    cnt++;
            }

            if (cnt)
                search.append(name, ii->first);
        }
    }



    return true;
}

bool hquotation_search_index::do_check_hqend_avg_end_le(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    std::vector<std::string> tmp_vec;
    std::vector<hidex_item> vec_idex;


    tmp_vec.push_back(value);

    //-1:1.0&-2:1
    for (uint32_t i = 0; i< tmp_vec.size(); i++)
    {
        std::vector<std::string> t_vec;
        SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 2) 
        { 
            continue;
        }

        hidex_item hi;
        hi.date_index = atoi(t_vec[0].c_str());
        hi.fpoint = atof(t_vec[1].c_str());
        vec_idex.push_back(hi);
    }

    int cnt = 0;

    if (search.empty())
    {
        for (auto ii = search_index->id_technical.begin(); ii != search_index->id_technical.end(); ii++)
        {
            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                if (tt[len - abs(vec_idex[i].date_index) - 1]->end_avg_end <= vec_idex[i].fpoint)
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
            auto ii = search_index->id_technical.find(*k);
            if (ii == search_index->id_technical.end())
                continue;

            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                if (tt[len - abs(vec_idex[i].date_index) - 1]->end_avg_end <= vec_idex[i].fpoint)
                    cnt++;
            }

            if (cnt)
                search.append(name, ii->first);
        }
    }



    return true;
}

bool hquotation_search_index::do_check_hqend_avg_end_ge_num_ge(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    std::vector<std::string> tmp_vec;
    std::vector<hidex_item> vec_idex;


    tmp_vec.push_back(value);

    //-5:0:2:1.0&-2:0:1:1
    for (uint32_t i = 0; i< tmp_vec.size(); i++)
    {
        std::vector<std::string> t_vec;
        SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 4) 
        { 
            continue;
        }

        hidex_item hi;
        hi.date_index = atoi(t_vec[0].c_str());
        hi.date_index_end = atoi(t_vec[1].c_str());
        hi.sum = atoi(t_vec[2].c_str());
        hi.fpoint = atof(t_vec[3].c_str());

        if (abs(hi.date_index) < abs(hi.date_index_end))
            continue;

        vec_idex.push_back(hi);
    }

    int cnt = 0;
    int count = 0;

    if (search.empty())
    {
        for (auto ii = search_index->id_technical.begin(); ii != search_index->id_technical.end(); ii++)
        {
            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                count = 0;               
                for (int k = len - abs(vec_idex[i].date_index) - 1; k <= len - abs(vec_idex[i].date_index_end) - 1; k++) {
                    if (tt[k]->end_avg_end >= vec_idex[i].fpoint)
                        count++;
                }

                if (count >= vec_idex[i].sum)
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
            auto ii = search_index->id_technical.find(*k);
            if (ii == search_index->id_technical.end())
                continue;

            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                count = 0;               
                for (int k = len - abs(vec_idex[i].date_index) - 1; k <= len - abs(vec_idex[i].date_index_end) - 1; k++) {
                    if (tt[k]->end_avg_end >= vec_idex[i].fpoint)
                        count++;
                }


                if (count >= vec_idex[i].sum)
                    cnt++;
            }

            if (cnt)
                search.append(name, ii->first);
        }
    }



    return true;
}

bool hquotation_search_index::do_check_id_substr(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    std::vector<std::string> tmp_vec;
    std::vector<hidex_item> vec_idex;


    tmp_vec.push_back(value);

    //300;60
    for (uint32_t i = 0; i< tmp_vec.size(); i++)
    {
        hidex_item hi;
        hi.sstr = tmp_vec[i];
        vec_idex.push_back(hi);
    }

    int cnt = 0;

    if (search.empty())
    {
        for (auto ii = search_index->id_quotation.begin(); ii != search_index->id_quotation.end(); ii++)
        {
            const std::string & id = ii->first;
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (strstr(id.c_str(), vec_idex[i].sstr.c_str()))
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
            auto ii = search_index->id_quotation.find(*k);
            if (ii == search_index->id_quotation.end())
                continue;

            const std::string & id = ii->first;
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (strstr(id.c_str(), vec_idex[i].sstr.c_str())) 
                    cnt++;
            }

            if (cnt)
                search.append(name, ii->first);
        }
    }



    return true;
}

bool hquotation_search_index::do_check_rlow_hlow_ge(std::string &name, std::string &value, search_res & search)                                              
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    std::vector<std::string> tmp_vec;
    std::vector<hidex_item> vec_idex;


    tmp_vec.push_back(value);

    //-1:1.0&-2:1
    for (uint32_t i = 0; i< tmp_vec.size(); i++)
    {
        std::vector<std::string> t_vec;
        SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 2) 
        { 
            continue;
        }

        hidex_item hi;
        hi.date_index = atoi(t_vec[0].c_str());
        hi.fpoint = atof(t_vec[1].c_str());
        vec_idex.push_back(hi);
    }

    int cnt = 0;

    if (search.empty())
    {
        for (auto ii = search_index->id_quotation.begin(); ii != search_index->id_quotation.end(); ii++)
        {
            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                if (tt[len - 1]->low >= vec_idex[i].fpoint * tt[len - abs(vec_idex[i].date_index) - 1]->low)
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
            auto ii = search_index->id_quotation.find(*k);
            if (ii == search_index->id_quotation.end())
                continue;

            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                if (tt[len - 1]->low >= vec_idex[i].fpoint * tt[len - abs(vec_idex[i].date_index) - 1]->low)
                    cnt++;
            }

            if (cnt)
                search.append(name, ii->first);
        }
    }



    return true;
}


bool hquotation_search_index::do_check_rlow_hlowest_ge(std::string &name, std::string &value, search_res & search)                                              
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    std::vector<std::string> tmp_vec;
    std::vector<hidex_item> vec_idex;


    tmp_vec.push_back(value);

    //-10:1:0:1.0:k1
    for (uint32_t i = 0; i< tmp_vec.size(); i++)
    {
        std::vector<std::string> t_vec;
        SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 3) 
        { 
            continue;
        }

        hidex_item hi;
        hi.date_index = atoi(t_vec[0].c_str());
        hi.date_index_end = atoi(t_vec[1].c_str());
        hi.fpoint = atof(t_vec[2].c_str());

        if (abs(hi.date_index) < abs(hi.date_index_end))
            continue;

        vec_idex.push_back(hi);
    }

    int cnt = 0;

    if (search.empty())
    {
        for (auto ii = search_index->id_quotation.begin(); ii != search_index->id_quotation.end(); ii++)
        {
            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                float min = tt[len - 1]->low;                                        
                for (int k = len - abs(vec_idex[i].date_index) - 1; k <= len  - abs(vec_idex[i].date_index_end) - 1; k++) 
                    if (min >= tt[k]->low)
                        min = tt[k]->low;

                if (tt[len - 1]->low >= vec_idex[i].fpoint * min)
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
            auto ii = search_index->id_quotation.find(*k);
            if (ii == search_index->id_quotation.end())
                continue;

            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                float min = tt[len - 1]->low;                                        
                for (int k = len - abs(vec_idex[i].date_index) - 1; k <= len  - abs(vec_idex[i].date_index_end) - 1; k++) 
                    if (min >= tt[k]->low)
                        min = tt[k]->low;

                if (tt[len - 1]->low >= vec_idex[i].fpoint * min)
                    cnt++;
            }

            if (cnt)
                search.append(name, ii->first);
        }
    }



    return true;
}


bool hquotation_search_index::do_check_rlow_hlowest_le(std::string &name, std::string &value, search_res & search)                                              
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    std::vector<std::string> tmp_vec;
    std::vector<hidex_item> vec_idex;


    tmp_vec.push_back(value);

    //-10:1:1.0;-5:1:1
    for (uint32_t i = 0; i< tmp_vec.size(); i++)
    {
        std::vector<std::string> t_vec;
        SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 3) 
        { 
            continue;
        }

        hidex_item hi;
        hi.date_index = atoi(t_vec[0].c_str());
        hi.date_index_end = atoi(t_vec[1].c_str());
        hi.fpoint = atof(t_vec[2].c_str());

        if (abs(hi.date_index) < abs(hi.date_index_end))
            continue;

        vec_idex.push_back(hi);
    }

    int cnt = 0;

    if (search.empty())
    {
        for (auto ii = search_index->id_quotation.begin(); ii != search_index->id_quotation.end(); ii++)
        {
            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                float min = tt[len - 1]->low;                                        
                for (int k = len - abs(vec_idex[i].date_index) - 1; k <= len - abs(vec_idex[i].date_index_end) - 1; k++) 
                    if (min >= tt[k]->low)
                        min = tt[k]->low;

                if (tt[len - 1]->low <= vec_idex[i].fpoint * min)
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
            auto ii = search_index->id_quotation.find(*k);
            if (ii == search_index->id_quotation.end())
                continue;

            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                float min = tt[len - 1]->low;                                        
                for (int k = len - abs(vec_idex[i].date_index) - 1; k <= len - abs(vec_idex[i].date_index_end) - 1; k++) 
                    if (min >= tt[k]->low)
                        min = tt[k]->low;

                if (tt[len - 1]->low <= vec_idex[i].fpoint * min)
                    cnt++;

            }

            if (cnt)
                search.append(name, ii->first);
        }
    }



    return true;
}


bool hquotation_search_index::do_check_rhigh_hhighest_ge(std::string &name, std::string &value, search_res & search)                                              
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    std::vector<std::string> tmp_vec;
    std::vector<hidex_item> vec_idex;


    tmp_vec.push_back(value);

    //-10:1:1.0;-5:1:1
    for (uint32_t i = 0; i< tmp_vec.size(); i++)
    {
        std::vector<std::string> t_vec;
        SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 3) 
        { 
            continue;
        }

        hidex_item hi;
        hi.date_index = atoi(t_vec[0].c_str());
        hi.date_index_end = atoi(t_vec[1].c_str());
        hi.fpoint = atof(t_vec[2].c_str());

        if (abs(hi.date_index) < abs(hi.date_index_end))
            continue;

        vec_idex.push_back(hi);
    }

    int cnt = 0;

    if (search.empty())
    {
        for (auto ii = search_index->id_quotation.begin(); ii != search_index->id_quotation.end(); ii++)
        {
            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                float max = tt[len -abs(vec_idex[i].date_index)- 1]->high;                                        
                for (int k = len - abs(vec_idex[i].date_index) - 1; k <= len  -abs(vec_idex[i].date_index_end)- 1; k++) 
                    if (max <= tt[k]->high)
                        max = tt[k]->high;

                if (tt[len - 1]->high >= vec_idex[i].fpoint * max)
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
            auto ii = search_index->id_quotation.find(*k);
            if (ii == search_index->id_quotation.end())
                continue;

            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                float max = tt[len -abs(vec_idex[i].date_index)- 1]->high;                                        
                for (int k = len - abs(vec_idex[i].date_index) - 1; k <= len -abs(vec_idex[i].date_index_end) - 1; k++) 
                    if (max <= tt[k]->high)
                        max = tt[k]->high;

                if (tt[len - 1]->high >= vec_idex[i].fpoint * max)
                    cnt++;
            }

            if (cnt)
                search.append(name, ii->first);
        }
    }



    return true;
}


bool hquotation_search_index::do_check_rhigh_hhighest_le(std::string &name, std::string &value, search_res & search)                                              
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    std::vector<std::string> tmp_vec;
    std::vector<hidex_item> vec_idex;


    tmp_vec.push_back(value);

    //-10:1:1.0;-5:1:1
    for (uint32_t i = 0; i< tmp_vec.size(); i++)
    {
        std::vector<std::string> t_vec;
        SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 3) 
        { 
            continue;
        }

        hidex_item hi;
        hi.date_index = atoi(t_vec[0].c_str());
        hi.date_index_end = atoi(t_vec[1].c_str());
        hi.fpoint = atof(t_vec[2].c_str());
        if (abs(hi.date_index) < abs(hi.date_index_end))
            continue;
        vec_idex.push_back(hi);
    }

    int cnt = 0;

    if (search.empty())
    {
        for (auto ii = search_index->id_quotation.begin(); ii != search_index->id_quotation.end(); ii++)
        {
            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                float max = tt[len -abs(vec_idex[i].date_index)- 1]->high;                                        
                for (int k = len - abs(vec_idex[i].date_index) - 1; k <= len  -abs(vec_idex[i].date_index_end)- 1; k++) 
                    if (max <= tt[k]->high)
                        max = tt[k]->high;

                if (tt[len - 1]->high <= vec_idex[i].fpoint * max)
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
            auto ii = search_index->id_quotation.find(*k);
            if (ii == search_index->id_quotation.end())
                continue;

            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                float max = tt[len -abs(vec_idex[i].date_index)- 1]->high;                                        
                for (int k = len - abs(vec_idex[i].date_index) - 1; k <= len  -abs(vec_idex[i].date_index_end)- 1; k++) 
                    if (max <= tt[k]->high)
                        max = tt[k]->high;

                if (tt[len - 1]->high <= vec_idex[i].fpoint * max)
                    cnt++;
            }

            if (cnt)
                search.append(name, ii->first);
        }
    }



    return true;
}


bool hquotation_search_index::do_check_rlow_hlow_le(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    std::vector<std::string> tmp_vec;
    std::vector<hidex_item> vec_idex;


    tmp_vec.push_back(value);

    //-1:1.0&-2:1
    for (uint32_t i = 0; i< tmp_vec.size(); i++)
    {
        std::vector<std::string> t_vec;
        SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 2) 
        { 
            continue;
        }

        hidex_item hi;
        hi.date_index = atoi(t_vec[0].c_str());
        hi.fpoint = atof(t_vec[1].c_str());
        vec_idex.push_back(hi);
    }

    int cnt = 0;

    if (search.empty())
    {
        for (auto ii = search_index->id_quotation.begin(); ii != search_index->id_quotation.end(); ii++)
        {
            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                if (tt[len - 1]->low <= vec_idex[i].fpoint * tt[len - abs(vec_idex[i].date_index) - 1]->low)
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
            auto ii = search_index->id_quotation.find(*k);
            if (ii == search_index->id_quotation.end())
                continue;

            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                if (tt[len - 1]->low <= vec_idex[i].fpoint * tt[len - abs(vec_idex[i].date_index) - 1]->low)
                    cnt++;
            }

            if (cnt)
                search.append(name, ii->first);
        }
    }



    return true;
}

bool hquotation_search_index::do_check_rlow_hlow_ge_num_ge(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    std::vector<std::string> tmp_vec;
    std::vector<hidex_item> vec_idex;


    tmp_vec.push_back(value);

    //-5:1:2:1.0&-2:1:1:1
    for (uint32_t i = 0; i< tmp_vec.size(); i++)
    {
        std::vector<std::string> t_vec;
        SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 4) 
        { 
            continue;
        }

        hidex_item hi;
        hi.date_index = atoi(t_vec[0].c_str());
        hi.date_index_end = atoi(t_vec[1].c_str());
        hi.sum = atoi(t_vec[2].c_str());
        hi.fpoint = atof(t_vec[3].c_str());

        if (abs(hi.date_index) < abs(hi.date_index_end))
            continue;

        vec_idex.push_back(hi);
    }

    int cnt = 0;
    int count = 0;

    if (search.empty())
    {
        for (auto ii = search_index->id_quotation.begin(); ii != search_index->id_quotation.end(); ii++)
        {
            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                count = 0;               
                for (int k = len - abs(vec_idex[i].date_index) - 1; k <= len - abs(vec_idex[i].date_index_end) - 1; k++) {
                    if (tt[len -1]->low >= vec_idex[i].fpoint * tt[k]->low)
                        count++;
                }

                if (count >= vec_idex[i].sum)
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
            auto ii = search_index->id_quotation.find(*k);
            if (ii == search_index->id_quotation.end())
                continue;

            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                count = 0;               
                for (int k = len - abs(vec_idex[i].date_index) - 1; k <= len - abs(vec_idex[i].date_index_end) - 1; k++) {
                    if (tt[len -1]->low >= vec_idex[i].fpoint * tt[k]->low)
                        count++;
                }


                if (count >= vec_idex[i].sum)
                    cnt++;
            }

            if (cnt)
                search.append(name, ii->first);
        }
    }



    return true;
}


bool hquotation_search_index::do_check_rhigh_hhigh_ge(std::string &name, std::string &value, search_res & search)                                              
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    std::vector<std::string> tmp_vec;
    std::vector<hidex_item> vec_idex;


    tmp_vec.push_back(value);

    //-1:1.0&-2:1
    for (uint32_t i = 0; i< tmp_vec.size(); i++)
    {
        std::vector<std::string> t_vec;
        SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 2) 
        { 
            continue;
        }

        hidex_item hi;
        hi.date_index = atoi(t_vec[0].c_str());
        hi.fpoint = atof(t_vec[1].c_str());
        vec_idex.push_back(hi);
    }

    int cnt = 0;

    if (search.empty())
    {
        for (auto ii = search_index->id_quotation.begin(); ii != search_index->id_quotation.end(); ii++)
        {
            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                if (tt[len - 1]->high >= vec_idex[i].fpoint * tt[len - abs(vec_idex[i].date_index) - 1]->high)
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
            auto ii = search_index->id_quotation.find(*k);
            if (ii == search_index->id_quotation.end())
                continue;

            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                if (tt[len - 1]->high >= vec_idex[i].fpoint * tt[len - abs(vec_idex[i].date_index) - 1]->high)
                    cnt++;
            }

            if (cnt)
                search.append(name, ii->first);

        }
    }



    return true;
}

bool hquotation_search_index::do_check_rhigh_hhigh_le(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    std::vector<std::string> tmp_vec;
    std::vector<hidex_item> vec_idex;


    tmp_vec.push_back(value);

    //-1:1.0&-2:1
    for (uint32_t i = 0; i< tmp_vec.size(); i++)
    {
        std::vector<std::string> t_vec;
        SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 2) 
        { 
            continue;
        }

        hidex_item hi;
        hi.date_index = atoi(t_vec[0].c_str());
        hi.fpoint = atof(t_vec[1].c_str());
        vec_idex.push_back(hi);
    }

    int cnt = 0;

    if (search.empty())
    {
        for (auto ii = search_index->id_quotation.begin(); ii != search_index->id_quotation.end(); ii++)
        {
            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                if (tt[len - 1]->high <= vec_idex[i].fpoint * tt[len - abs(vec_idex[i].date_index) - 1]->high)
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
            auto ii = search_index->id_quotation.find(*k);
            if (ii == search_index->id_quotation.end())
                continue;

            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                if (tt[len - 1]->high <= vec_idex[i].fpoint * tt[len - abs(vec_idex[i].date_index) - 1]->high)
                    cnt++;
            }

            if (cnt)
                search.append(name, ii->first);
        }
    }



    return true;
}

bool hquotation_search_index::do_check_rhigh_hhigh_ge_num_ge(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    std::vector<std::string> tmp_vec;
    std::vector<hidex_item> vec_idex;


    tmp_vec.push_back(value);

    //-5:1:2:1.0&-2:1:1:1
    for (uint32_t i = 0; i< tmp_vec.size(); i++)
    {
        std::vector<std::string> t_vec;
        SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 4) 
        { 
            continue;
        }

        hidex_item hi;
        hi.date_index = atoi(t_vec[0].c_str());
        hi.date_index_end = atoi(t_vec[1].c_str());
        hi.sum = atoi(t_vec[2].c_str());
        hi.fpoint = atof(t_vec[3].c_str());

        if (abs(hi.date_index) < abs(hi.date_index_end))
            continue;

        vec_idex.push_back(hi);
    }

    int cnt = 0;
    int count = 0;

    if (search.empty())
    {
        for (auto ii = search_index->id_quotation.begin(); ii != search_index->id_quotation.end(); ii++)
        {
            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                count = 0;               
                for (int k = len - abs(vec_idex[i].date_index) - 1; k <= len - abs(vec_idex[i].date_index_end) - 1; k++) {
                    if (tt[len -1]->high >= vec_idex[i].fpoint * tt[k]->high)
                        count++;
                }

                if (count >= vec_idex[i].sum)
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
            auto ii = search_index->id_quotation.find(*k);
            if (ii == search_index->id_quotation.end())
                continue;

            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                count = 0;               
                for (int k = len - abs(vec_idex[i].date_index) - 1; k <= len - abs(vec_idex[i].date_index_end) - 1; k++) {
                    if (tt[len -1]->high >= vec_idex[i].fpoint * tt[k]->high)
                        count++;
                }


                if (count >= vec_idex[i].sum)
                    cnt++;
            }

            if (cnt)
                search.append(name, ii->first);
        }
    }



    return true;
}

bool hquotation_search_index::do_check_hqend_end_5_ge(std::string &name, std::string &value, search_res & search)                                              
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    std::vector<std::string> tmp_vec;
    std::vector<hidex_item> vec_idex;


    tmp_vec.push_back(value);

    //-1:1.0&-2:1
    for (uint32_t i = 0; i< tmp_vec.size(); i++)
    {
        std::vector<std::string> t_vec;
        SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 2) 
        { 
            continue;
        }

        hidex_item hi;
        hi.date_index = atoi(t_vec[0].c_str());
        hi.fpoint = atof(t_vec[1].c_str());
        vec_idex.push_back(hi);
    }

    int cnt = 0;

    if (search.empty())
    {
        for (auto ii = search_index->id_technical.begin(); ii != search_index->id_technical.end(); ii++)
        {
            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                if (tt[len - abs(vec_idex[i].date_index) - 1]->end_end_5 >= vec_idex[i].fpoint)
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
            auto ii = search_index->id_technical.find(*k);
            if (ii == search_index->id_technical.end())
                continue;

            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                if (tt[len - abs(vec_idex[i].date_index) - 1]->end_end_5 >= vec_idex[i].fpoint)
                    cnt++;
            }

            if (cnt)
                search.append(name, ii->first);
        }
    }



    return true;
}

bool hquotation_search_index::do_check_hqend_end_5_le(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    std::vector<std::string> tmp_vec;
    std::vector<hidex_item> vec_idex;


    tmp_vec.push_back(value);

    //-1:1.0&-2:1
    for (uint32_t i = 0; i< tmp_vec.size(); i++)
    {
        std::vector<std::string> t_vec;
        SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 2) 
        { 
            continue;
        }

        hidex_item hi;
        hi.date_index = atoi(t_vec[0].c_str());
        hi.fpoint = atof(t_vec[1].c_str());
        vec_idex.push_back(hi);
    }

    int cnt = 0;

    if (search.empty())
    {
        for (auto ii = search_index->id_technical.begin(); ii != search_index->id_technical.end(); ii++)
        {
            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                if (tt[len - abs(vec_idex[i].date_index) - 1]->end_end_5 <= vec_idex[i].fpoint)
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
            auto ii = search_index->id_technical.find(*k);
            if (ii == search_index->id_technical.end())
                continue;

            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                if (tt[len - abs(vec_idex[i].date_index) - 1]->end_end_5 <= vec_idex[i].fpoint)
                    cnt++;
            }

            if (cnt)
                search.append(name, ii->first);
        }
    }



    return true;
}

bool hquotation_search_index::do_check_hqend_end_5_ge_num_ge(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    std::vector<std::string> tmp_vec;
    std::vector<hidex_item> vec_idex;


    tmp_vec.push_back(value);

    //-5:0:2:1.0&-2:0:1:1
    for (uint32_t i = 0; i< tmp_vec.size(); i++)
    {
        std::vector<std::string> t_vec;
        SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 4) 
        { 
            continue;
        }

        hidex_item hi;
        hi.date_index = atoi(t_vec[0].c_str());
        hi.date_index_end = atoi(t_vec[1].c_str());
        hi.sum = atoi(t_vec[2].c_str());
        hi.fpoint = atof(t_vec[3].c_str());

        if (abs(hi.date_index) < abs(hi.date_index_end))
            continue;

        vec_idex.push_back(hi);
    }

    int cnt = 0;
    int count = 0;

    if (search.empty())
    {
        for (auto ii = search_index->id_technical.begin(); ii != search_index->id_technical.end(); ii++)
        {
            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                count = 0;               
                for (int k = len - abs(vec_idex[i].date_index) - 1; k <= len - abs(vec_idex[i].date_index_end) - 1; k++) {
                    if (tt[k]->end_end_5 >= vec_idex[i].fpoint)
                        count++;
                }

                if (count >= vec_idex[i].sum)
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
            auto ii = search_index->id_technical.find(*k);
            if (ii == search_index->id_technical.end())
                continue;

            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                count = 0;               
                for (int k = len - abs(vec_idex[i].date_index) - 1; k <= len - abs(vec_idex[i].date_index_end) - 1; k++) {
                    if (tt[k]->end_end_5 >= vec_idex[i].fpoint)
                        count++;
                }


                if (count >= vec_idex[i].sum)
                    cnt++;
            }

            if (cnt)
                search.append(name, ii->first);
        }
    }



    return true;
}


bool hquotation_search_index::do_check_hqend_end_10_ge(std::string &name, std::string &value, search_res & search)                                              
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    std::vector<std::string> tmp_vec;
    std::vector<hidex_item> vec_idex;


    tmp_vec.push_back(value);

    //-1:1.0&-2:1
    for (uint32_t i = 0; i< tmp_vec.size(); i++)
    {
        std::vector<std::string> t_vec;
        SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 2) 
        { 
            continue;
        }

        hidex_item hi;
        hi.date_index = atoi(t_vec[0].c_str());
        hi.fpoint = atof(t_vec[1].c_str());
        vec_idex.push_back(hi);
    }

    int cnt = 0;

    if (search.empty())
    {
        for (auto ii = search_index->id_technical.begin(); ii != search_index->id_technical.end(); ii++)
        {
            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                if (tt[len - abs(vec_idex[i].date_index) - 1]->end_end_10 >= vec_idex[i].fpoint)
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
            auto ii = search_index->id_technical.find(*k);
            if (ii == search_index->id_technical.end())
                continue;

            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                if (tt[len - abs(vec_idex[i].date_index) - 1]->end_end_10 >= vec_idex[i].fpoint)
                    cnt++;
            }

            if (cnt)
                search.append(name, ii->first);
        }
    }



    return true;
}

bool hquotation_search_index::do_check_hqend_end_10_le(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    std::vector<std::string> tmp_vec;
    std::vector<hidex_item> vec_idex;


    tmp_vec.push_back(value);

    //-1:1.0&-2:1
    for (uint32_t i = 0; i< tmp_vec.size(); i++)
    {
        std::vector<std::string> t_vec;
        SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 2) 
        { 
            continue;
        }

        hidex_item hi;
        hi.date_index = atoi(t_vec[0].c_str());
        hi.fpoint = atof(t_vec[1].c_str());
        vec_idex.push_back(hi);
    }

    int cnt = 0;

    if (search.empty())
    {
        for (auto ii = search_index->id_technical.begin(); ii != search_index->id_technical.end(); ii++)
        {
            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                if (tt[len - abs(vec_idex[i].date_index) - 1]->end_end_10 <= vec_idex[i].fpoint)
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
            auto ii = search_index->id_technical.find(*k);
            if (ii == search_index->id_technical.end())
                continue;

            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                if (tt[len - abs(vec_idex[i].date_index) - 1]->end_end_10 <= vec_idex[i].fpoint)
                    cnt++;
            }

            if (cnt)
                search.append(name, ii->first);
        }
    }



    return true;
}

bool hquotation_search_index::do_check_hqend_end_10_ge_num_ge(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    std::vector<std::string> tmp_vec;
    std::vector<hidex_item> vec_idex;


    tmp_vec.push_back(value);

    //-5:0:2:1.0&-2:0:1:1
    for (uint32_t i = 0; i< tmp_vec.size(); i++)
    {
        std::vector<std::string> t_vec;
        SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 4) 
        { 
            continue;
        }

        hidex_item hi;
        hi.date_index = atoi(t_vec[0].c_str());
        hi.date_index_end = atoi(t_vec[1].c_str());
        hi.sum = atoi(t_vec[2].c_str());
        hi.fpoint = atof(t_vec[3].c_str());

        if (abs(hi.date_index) < abs(hi.date_index_end))
            continue;

        vec_idex.push_back(hi);
    }

    int cnt = 0;
    int count = 0;

    if (search.empty())
    {
        for (auto ii = search_index->id_technical.begin(); ii != search_index->id_technical.end(); ii++)
        {
            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                count = 0;               
                for (int k = len - abs(vec_idex[i].date_index) - 1; k <= len - abs(vec_idex[i].date_index_end) - 1; k++) {
                    if (tt[k]->end_end_10 >= vec_idex[i].fpoint)
                        count++;
                }

                if (count >= vec_idex[i].sum)
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
            auto ii = search_index->id_technical.find(*k);
            if (ii == search_index->id_technical.end())
                continue;

            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                count = 0;               
                for (int k = len - abs(vec_idex[i].date_index) - 1; k <= len - abs(vec_idex[i].date_index_end) - 1; k++) {
                    if (tt[k]->end_end_10 >= vec_idex[i].fpoint)
                        count++;
                }


                if (count >= vec_idex[i].sum)
                    cnt++;
            }

            if (cnt)
                search.append(name, ii->first);
        }
    }



    return true;
}

bool hquotation_search_index::do_check_hqend_end_20_ge(std::string &name, std::string &value, search_res & search)                                              
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    std::vector<std::string> tmp_vec;
    std::vector<hidex_item> vec_idex;


    tmp_vec.push_back(value);

    //-1:1.0&-2:1
    for (uint32_t i = 0; i< tmp_vec.size(); i++)
    {
        std::vector<std::string> t_vec;
        SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 2) 
        { 
            continue;
        }

        hidex_item hi;
        hi.date_index = atoi(t_vec[0].c_str());
        hi.fpoint = atof(t_vec[1].c_str());
        vec_idex.push_back(hi);
    }

    int cnt = 0;

    if (search.empty())
    {
        for (auto ii = search_index->id_technical.begin(); ii != search_index->id_technical.end(); ii++)
        {
            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                if (tt[len - abs(vec_idex[i].date_index) - 1]->end_end_20 >= vec_idex[i].fpoint)
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
            auto ii = search_index->id_technical.find(*k);
            if (ii == search_index->id_technical.end())
                continue;

            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                if (tt[len - abs(vec_idex[i].date_index) - 1]->end_end_20 >= vec_idex[i].fpoint)
                    cnt++;
            }

            if (cnt)
                search.append(name, ii->first);
        }
    }



    return true;
}

bool hquotation_search_index::do_check_hqend_end_20_le(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    std::vector<std::string> tmp_vec;
    std::vector<hidex_item> vec_idex;


    tmp_vec.push_back(value);

    //-1:1.0&-2:1
    for (uint32_t i = 0; i< tmp_vec.size(); i++)
    {
        std::vector<std::string> t_vec;
        SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 2) 
        { 
            continue;
        }

        hidex_item hi;
        hi.date_index = atoi(t_vec[0].c_str());
        hi.fpoint = atof(t_vec[1].c_str());
        vec_idex.push_back(hi);
    }

    int cnt = 0;

    if (search.empty())
    {
        for (auto ii = search_index->id_technical.begin(); ii != search_index->id_technical.end(); ii++)
        {
            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                if (tt[len - abs(vec_idex[i].date_index) - 1]->end_end_20 <= vec_idex[i].fpoint)
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
            auto ii = search_index->id_technical.find(*k);
            if (ii == search_index->id_technical.end())
                continue;

            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                if (tt[len - abs(vec_idex[i].date_index) - 1]->end_end_20 <= vec_idex[i].fpoint)
                    cnt++;
            }

            if (cnt)
                search.append(name, ii->first);
        }
    }



    return true;
}

bool hquotation_search_index::do_check_hqend_end_20_ge_num_ge(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    SETS_OP_TRPE tmp_ot;
    proc_data* p_data = proc_data::instance();

    std::vector<std::string> tmp_vec;
    std::vector<hidex_item> vec_idex;


    tmp_vec.push_back(value);

    //-5:0:2:1.0&-2:0:1:1
    for (uint32_t i = 0; i< tmp_vec.size(); i++)
    {
        std::vector<std::string> t_vec;
        SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 4) 
        { 
            continue;
        }

        hidex_item hi;
        hi.date_index = atoi(t_vec[0].c_str());
        hi.date_index_end = atoi(t_vec[1].c_str());
        hi.sum = atoi(t_vec[2].c_str());
        hi.fpoint = atof(t_vec[3].c_str());

        if (abs(hi.date_index) < abs(hi.date_index_end))
            continue;

        vec_idex.push_back(hi);
    }

    int cnt = 0;
    int count = 0;

    if (search.empty())
    {
        for (auto ii = search_index->id_technical.begin(); ii != search_index->id_technical.end(); ii++)
        {
            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                count = 0;               
                for (int k = len - abs(vec_idex[i].date_index) - 1; k <= len - abs(vec_idex[i].date_index_end) - 1; k++) {
                    if (tt[k]->end_end_20 >= vec_idex[i].fpoint)
                        count++;
                }

                if (count >= vec_idex[i].sum)
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
            auto ii = search_index->id_technical.find(*k);
            if (ii == search_index->id_technical.end())
                continue;

            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                count = 0;               
                for (int k = len - abs(vec_idex[i].date_index) - 1; k <= len - abs(vec_idex[i].date_index_end) - 1; k++) {
                    if (tt[k]->end_end_20 >= vec_idex[i].fpoint)
                        count++;
                }


                if (count >= vec_idex[i].sum)
                    cnt++;
            }

            if (cnt)
                search.append(name, ii->first);
        }
    }



    return true;
}

bool hquotation_search_index::do_check_hqend_end_30_ge(std::string &name, std::string &value, search_res & search)                                              
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    std::vector<std::string> tmp_vec;
    std::vector<hidex_item> vec_idex;


    tmp_vec.push_back(value);

    //-1:1.0&-2:1
    for (uint32_t i = 0; i< tmp_vec.size(); i++)
    {
        std::vector<std::string> t_vec;
        SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 2) 
        { 
            continue;
        }

        hidex_item hi;
        hi.date_index = atoi(t_vec[0].c_str());
        hi.fpoint = atof(t_vec[1].c_str());
        vec_idex.push_back(hi);
    }

    int cnt = 0;

    if (search.empty())
    {
        for (auto ii = search_index->id_technical.begin(); ii != search_index->id_technical.end(); ii++)
        {
            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                if (tt[len - abs(vec_idex[i].date_index) - 1]->end_end_30 >= vec_idex[i].fpoint)
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
            auto ii = search_index->id_technical.find(*k);
            if (ii == search_index->id_technical.end())
                continue;

            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                if (tt[len - abs(vec_idex[i].date_index) - 1]->end_end_30 >= vec_idex[i].fpoint)
                    cnt++;
            }

            if (cnt)
                search.append(name, ii->first);
        }
    }



    return true;
}

bool hquotation_search_index::do_check_hqend_end_30_le(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    std::vector<std::string> tmp_vec;
    std::vector<hidex_item> vec_idex;


    tmp_vec.push_back(value);

    //-1:1.0&-2:1
    for (uint32_t i = 0; i< tmp_vec.size(); i++)
    {
        std::vector<std::string> t_vec;
        SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 2) 
        { 
            continue;
        }

        hidex_item hi;
        hi.date_index = atoi(t_vec[0].c_str());
        hi.fpoint = atof(t_vec[1].c_str());
        vec_idex.push_back(hi);
    }

    int cnt = 0;

    if (search.empty())
    {
        for (auto ii = search_index->id_technical.begin(); ii != search_index->id_technical.end(); ii++)
        {
            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                if (tt[len - abs(vec_idex[i].date_index) - 1]->end_end_30 <= vec_idex[i].fpoint)
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
            auto ii = search_index->id_technical.find(*k);
            if (ii == search_index->id_technical.end())
                continue;

            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                if (tt[len - abs(vec_idex[i].date_index) - 1]->end_end_30 <= vec_idex[i].fpoint)
                    cnt++;
            }

            if (cnt)
                search.append(name, ii->first);
        }
    }



    return true;
}

bool hquotation_search_index::do_check_hqend_end_30_ge_num_ge(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    std::vector<std::string> tmp_vec;
    std::vector<hidex_item> vec_idex;


    tmp_vec.push_back(value);

    //-5:0:2:1.0&-2:0:1:1
    for (uint32_t i = 0; i< tmp_vec.size(); i++)
    {
        std::vector<std::string> t_vec;
        SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 4) 
        { 
            continue;
        }

        hidex_item hi;
        hi.date_index = atoi(t_vec[0].c_str());
        hi.date_index_end = atoi(t_vec[1].c_str());
        hi.sum = atoi(t_vec[2].c_str());
        hi.fpoint = atof(t_vec[3].c_str());

        if (abs(hi.date_index) < abs(hi.date_index_end))
            continue;

        vec_idex.push_back(hi);
    }

    int cnt = 0;
    int count = 0;

    if (search.empty())
    {
        for (auto ii = search_index->id_technical.begin(); ii != search_index->id_technical.end(); ii++)
        {
            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                count = 0;               
                for (int k = len - abs(vec_idex[i].date_index) - 1; k <= len - abs(vec_idex[i].date_index_end) - 1; k++) {
                    if (tt[k]->end_end_30 >= vec_idex[i].fpoint)
                        count++;
                }

                if (count >= vec_idex[i].sum)
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
            auto ii = search_index->id_technical.find(*k);
            if (ii == search_index->id_technical.end())
                continue;

            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                count = 0;               
                for (int k = len - abs(vec_idex[i].date_index) - 1; k <= len - abs(vec_idex[i].date_index_end) - 1; k++) {
                    if (tt[k]->end_end_30 >= vec_idex[i].fpoint)
                        count++;
                }


                if (count >= vec_idex[i].sum)
                    cnt++;
            }

            if (cnt)
                search.append(name, ii->first);
        }
    }



    return true;
}

bool hquotation_search_index::do_check_hqredvol_greenvol_ge(std::string &name, std::string &value, search_res & search)                                              
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    std::vector<std::string> tmp_vec;
    std::vector<hidex_item> vec_idex;

    tmp_vec.push_back(value);

    //-1:0:1.0&-2:0:1
    for (uint32_t i = 0; i< tmp_vec.size(); i++)
    {
        std::vector<std::string> t_vec;
        SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 3) 
        { 
            continue;
        }

        hidex_item hi;
        hi.date_index = atoi(t_vec[0].c_str());
        hi.date_index_end = atoi(t_vec[1].c_str());
        hi.fpoint = atof(t_vec[2].c_str());

        if (abs(hi.date_index) < abs(hi.date_index_end))
            continue;

        vec_idex.push_back(hi);
    }

    int cnt = 0;

    float red_change_rate, green_change_rate;
    red_change_rate = green_change_rate = 0;
    if (search.empty())
    {
        for (auto ii = search_index->id_quotation.begin(); ii != search_index->id_quotation.end(); ii++)
        {
            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                red_change_rate = green_change_rate = 0;
                for (int k = len - abs(vec_idex[i].date_index) - 1; k <= len - abs(vec_idex[i].date_index_end) - 1; k++) 
                {
                    if (tt[k]->end >= tt[k]->start)
                    {
                        red_change_rate += tt[k]->change_rate;
                    }
                    else
                    {
                        green_change_rate += tt[k]->change_rate;
                    }
                }

                if (red_change_rate >= green_change_rate * vec_idex[i].fpoint)
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
            auto ii = search_index->id_quotation.find(*k);
            if (ii == search_index->id_quotation.end())
                continue;

            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                red_change_rate = green_change_rate = 0;

                for (int k = len - abs(vec_idex[i].date_index) - 1; k <= len - abs(vec_idex[i].date_index_end) - 1; k++) 
                {    
                    if (tt[k]->end >= tt[k]->start)
                    {    
                        red_change_rate += tt[k]->change_rate;
                    }    
                    else 
                    {    
                        green_change_rate += tt[k]->change_rate;
                    }    
                }    

                if (red_change_rate >= green_change_rate * vec_idex[i].fpoint)
                    cnt++;
            }

            if (cnt)
                search.append(name, ii->first);
        }
    }



    return true;
}

bool hquotation_search_index::do_check_hqredvol_greenvol_le(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    SETS_OP_TRPE tmp_ot;
    proc_data* p_data = proc_data::instance();

    std::vector<std::string> tmp_vec;
    std::vector<hidex_item> vec_idex;

    tmp_vec.push_back(value);

    //-1:0:1.0&-2:0:1
    for (uint32_t i = 0; i< tmp_vec.size(); i++)
    {
        std::vector<std::string> t_vec;
        SplitString(tmp_vec[i].c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 3) 
        { 
            continue;
        }

        hidex_item hi;
        hi.date_index = atoi(t_vec[0].c_str());
        hi.date_index_end = atoi(t_vec[1].c_str());
        hi.fpoint = atof(t_vec[2].c_str());

        if (abs(hi.date_index) < abs(hi.date_index_end))
            continue;

        vec_idex.push_back(hi);
    }

    int cnt = 0;

    float red_change_rate, green_change_rate;
    red_change_rate = green_change_rate = 0;

    if (search.empty())
    {
        for (auto ii = search_index->id_quotation.begin(); ii != search_index->id_quotation.end(); ii++)
        {
            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                red_change_rate = green_change_rate = 0;
                for (int k = len - abs(vec_idex[i].date_index) - 1; k <= len - abs(vec_idex[i].date_index_end) - 1; k++) 
                {
                    if (tt[k]->end >= tt[k]->start)
                    {
                        red_change_rate += tt[k]->change_rate;
                    }
                    else
                    {
                        green_change_rate += tt[k]->change_rate;
                    }
                }

                if (red_change_rate <= green_change_rate * vec_idex[i].fpoint)
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
            auto ii = search_index->id_quotation.find(*k);
            if (ii == search_index->id_quotation.end())
                continue;

            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();
            cnt = 0;
            for (int i = 0; i < (int)vec_idex.size(); i++) 
            {
                if (len  < 1 + abs(vec_idex[i].date_index))
                    continue;

                red_change_rate = green_change_rate = 0;
                for (int k = len - abs(vec_idex[i].date_index) - 1; k <= len - abs(vec_idex[i].date_index_end) - 1; k++) 
                {
                    if (tt[k]->end >= tt[k]->start)
                    {
                        red_change_rate += tt[k]->change_rate;
                    }
                    else
                    {
                        green_change_rate += tt[k]->change_rate;
                    }
                }

                if (red_change_rate <= green_change_rate * vec_idex[i].fpoint)
                    cnt++;
            }

            if (cnt)
                search.append(name, ii->first);
        }
    }



    return true;
}

