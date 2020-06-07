#include "hquotation_search_index.h"
#include "sk_util.h"
#include "proc_data.h"
#include "history_quotation_dict.h"

bool hquotation_search_index::do_hqchange_rate_le(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    hidex_item hi;

    //-1:1.0&-2:1
    {
        std::vector<std::string> t_vec;
        SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 2) 
        { 
            return false;
        }

        hi.date_index = abs(atoi(t_vec[0].c_str()));
        get_first_not_numstr(t_vec[0], hi.start_str);

        hi.fpoint = atof(t_vec[1].c_str());
    }

    if (search.empty())
    {
        for (auto ii = search_index->id_quotation.begin(); ii != search_index->id_quotation.end(); ii++)
        {
            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();

            std::set<int> date_index_set {hi.date_index};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
                for (auto date_index: date_index_set)
                {
                    if (len  < 1 + date_index)
                        continue;

                    if (tt[len - date_index - 1]->change_rate <= hi.fpoint)
                        search.append(name, ii->first, date_index);
                }
            }
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

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            for (auto date_index: date_index_set)
            {
                if (len  < 1 + date_index)
                    continue;

                if (tt[len - date_index - 1]->change_rate <= hi.fpoint)
                    search.append(name, ii->first, date_index);
            }
        }
    }


    return true;
}

bool hquotation_search_index::do_hqchange_rate_ge(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    hidex_item hi;

    //-1:1.0&-2:1
    {
        std::vector<std::string> t_vec;
        SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 2) 
        { 
            return false;
        }

        hi.date_index = abs(atoi(t_vec[0].c_str()));
        get_first_not_numstr(t_vec[0], hi.start_str);
        hi.fpoint = atof(t_vec[1].c_str());
    }

    if (search.empty())
    {
        for (auto ii = search_index->id_quotation.begin(); ii != search_index->id_quotation.end(); ii++)
        {
            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            for (auto date_index: date_index_set)
            {
                if (len  < 1 + date_index)
                    continue;

                if (tt[len - date_index - 1]->change_rate >= hi.fpoint)
                    search.append(name, ii->first, date_index);
            }
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

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            for (auto date_index: date_index_set)
            {
                if (len  < 1 + date_index)
                    continue;

                if (tt[len - date_index - 1]->change_rate >= hi.fpoint)
                    search.append(name, ii->first, date_index);
            }
        }
    }


    return true;
}

bool hquotation_search_index::do_hqrange_percent_le(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    hidex_item hi;

    //-1:1.0&-2:1
    {
        std::vector<std::string> t_vec;
        SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 2) 
        { 
            return false;
        }

        hi.date_index = abs(atoi(t_vec[0].c_str()));
        get_first_not_numstr(t_vec[0], hi.start_str);
        hi.fpoint = atof(t_vec[1].c_str());
    }

    if (search.empty())
    {
        for (auto ii = search_index->id_quotation.begin(); ii != search_index->id_quotation.end(); ii++)
        {
            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            for (auto date_index: date_index_set)
            {
                if (len  < 1 + date_index)
                    continue;

                if (tt[len - date_index - 1]->range_percent <= hi.fpoint)
                    search.append(name, ii->first, date_index);
            }
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


            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            for (auto date_index: date_index_set)
            {
                if (len  < 1 + date_index)
                    continue;

                if (tt[len - date_index - 1]->range_percent <= hi.fpoint)
                    search.append(name, ii->first, date_index);
            }
        }
    }

    return true;
}

bool hquotation_search_index::do_hqrange_percent_ge(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    hidex_item hi;

    //-1:1.0&-2:1
    {
        std::vector<std::string> t_vec;
        SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 2) 
        { 
            return false;
        }

        hi.date_index = abs(atoi(t_vec[0].c_str()));
        get_first_not_numstr(t_vec[0], hi.start_str);
        hi.fpoint = atof(t_vec[1].c_str());
    }

    if (search.empty())
    {
        for (auto ii = search_index->id_quotation.begin(); ii != search_index->id_quotation.end(); ii++)
        {
            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            for (auto date_index: date_index_set)
            {
                if (len  < 1 + date_index)
                    continue;

                if (tt[len - date_index - 1]->range_percent >= hi.fpoint)
                    search.append(name, ii->first, date_index);
            }
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

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            for (auto date_index: date_index_set)
            {
                if (len  < 1 + date_index)
                    continue;

                if (tt[len - date_index - 1]->range_percent >= hi.fpoint)
                    search.append(name, ii->first, date_index);
            }
        }
    }

    return true;
}

bool hquotation_search_index::do_hqrange_percent_ge_num_ge(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    hidex_item hi;

    //-5:0:2:1.0&-2:0:1:1
    {
        std::vector<std::string> t_vec;
        SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 4) 
        { 
            return false;
        }

        hi.date_index = abs(atoi(t_vec[0].c_str()));
        get_first_not_numstr(t_vec[0], hi.start_str);

        hi.date_index_end = abs(atoi(t_vec[1].c_str()));
        get_first_not_numstr(t_vec[1], hi.end_str);

        hi.sum = atoi(t_vec[2].c_str());
        hi.fpoint = atof(t_vec[3].c_str());
    }

    int count = 0;
    if (search.empty())
    {
        for (auto ii = search_index->id_quotation.begin(); ii != search_index->id_quotation.end(); ii++)
        {
            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    count = 0;
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;

                    for (int k = len - date_index - 1; k <= len - date_index_end - 1; k++) {
                        if (tt[k]->range_percent >= hi.fpoint) {
                            count++;
                            index.insert(len - k - 1);
                        }
                    }

                    if (count >= hi.sum)
                        search.append(name, ii->first, index);
                }
            }
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

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    count = 0;
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;

                    for (int k = len - date_index - 1; k <= len - date_index_end - 1; k++) {
                        if (tt[k]->range_percent >= hi.fpoint) {
                            count++;
                            index.insert(len - k - 1);
                        }
                    }

                    if (count >= hi.sum)
                        search.append(name, ii->first, index);
                }
            }
        }
    }

    return true;
}

bool hquotation_search_index::do_hqrange_percent_le_num_ge(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    hidex_item hi;

    //-5:0:2:1.0&-2:0:1:1
    {
        std::vector<std::string> t_vec;
        SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 4) 
        { 
            return false;
        }

        hi.date_index = abs(atoi(t_vec[0].c_str()));
        get_first_not_numstr(t_vec[0], hi.start_str);

        hi.date_index_end = abs(atoi(t_vec[1].c_str()));
        get_first_not_numstr(t_vec[1], hi.end_str);

        hi.sum = atoi(t_vec[2].c_str());
        hi.fpoint = atof(t_vec[3].c_str());
    }

    int count = 0;
    if (search.empty())
    {
        for (auto ii = search_index->id_quotation.begin(); ii != search_index->id_quotation.end(); ii++)
        {
            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    count = 0;
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;

                    for (int k = len - date_index - 1; k <= len - date_index_end - 1; k++) {
                        if (tt[k]->range_percent <= hi.fpoint) {
                            count++;
                            index.insert(len - k - 1);
                        }
                    }

                    if (count >= hi.sum)
                        search.append(name, ii->first, index);
                }
            }
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

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    count = 0;
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;

                    for (int k = len - date_index - 1; k <= len - date_index_end - 1; k++) {
                        if (tt[k]->range_percent <= hi.fpoint) {
                            count++;
                            index.insert(len - k - 1);
                        }
                    }

                    if (count >= hi.sum)
                        search.append(name, ii->first, index);
                }
            }
        }
    }


    return true;
}

bool hquotation_search_index::do_hq_sum_change_rate_le(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    hidex_item hi;

    //-1:0:1.0&-2:0:1
    {
        std::vector<std::string> t_vec;
        SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 3) 
        { 
            return false;
        }

        hi.date_index = abs(atoi(t_vec[0].c_str()));
        get_first_not_numstr(t_vec[0], hi.start_str);

        hi.date_index_end = abs(atoi(t_vec[1].c_str()));
        get_first_not_numstr(t_vec[1], hi.end_str);

        hi.fpoint = atof(t_vec[2].c_str());
    }

    float count = 0;
    if (search.empty())
    {
        for (auto ii = search_index->id_sum_quotation.begin(); ii != search_index->id_sum_quotation.end(); ii++)
        {
            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    count = 0;
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;


                    int k = len - date_index - 2;
                    count = tt[len - date_index_end -1]->change_rate - tt[k]->change_rate;

                    index.insert(date_index);
                    index.insert(date_index_end);

                    if (count <= hi.fpoint)
                        search.append(name, ii->first, index);
                }
            }
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

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    count = 0;
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;


                    int k = len - date_index - 2;
                    count = tt[len - date_index_end -1]->change_rate - tt[k]->change_rate;

                    index.insert(date_index);
                    index.insert(date_index_end);

                    if (count <= hi.fpoint)
                        search.append(name, ii->first, index);
                }
            }
        }
    }


    return true;
}

bool hquotation_search_index::do_hq_sum_change_rate_ge(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    hidex_item hi;

    //-1:0:1.0&-2:0:1
    {
        std::vector<std::string> t_vec;
        SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 3) 
        { 
            return false;
        }

        hi.date_index = abs(atoi(t_vec[0].c_str()));
        get_first_not_numstr(t_vec[0], hi.start_str);

        hi.date_index_end = abs(atoi(t_vec[1].c_str()));
        get_first_not_numstr(t_vec[1], hi.end_str);

        hi.fpoint = atof(t_vec[2].c_str());
    }

    float count = 0;
    if (search.empty())
    {
        for (auto ii = search_index->id_sum_quotation.begin(); ii != search_index->id_sum_quotation.end(); ii++)
        {
            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    count = 0;
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;


                    int k = len - date_index - 2;
                    count = tt[len - date_index_end -1]->change_rate - tt[k]->change_rate;

                    index.insert(date_index);
                    index.insert(date_index_end);

                    if (count >= hi.fpoint)
                        search.append(name, ii->first, index);
                }
            }
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

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    count = 0;
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;


                    int k = len - date_index - 2;
                    count = tt[len - date_index_end -1]->change_rate - tt[k]->change_rate;

                    index.insert(date_index);
                    index.insert(date_index_end);

                    if (count >= hi.fpoint)
                        search.append(name, ii->first, index);
                }
            }
        }
    }


    return true;
}

bool hquotation_search_index::do_hq_sum_range_percent_le(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    hidex_item hi;

    //-1:0:1.0&-2:0:1
    {
        std::vector<std::string> t_vec;
        SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 3) 
        { 
            return false;
        }

        hi.date_index = abs(atoi(t_vec[0].c_str()));
        get_first_not_numstr(t_vec[0], hi.start_str);
        hi.date_index_end = abs(atoi(t_vec[1].c_str()));
        get_first_not_numstr(t_vec[1], hi.end_str);

        hi.fpoint = atof(t_vec[2].c_str());
    }

    float count = 0;
    if (search.empty())
    {
        for (auto ii = search_index->id_sum_quotation.begin(); ii != search_index->id_sum_quotation.end(); ii++)
        {
            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    count = 0;
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;


                    int k = len - date_index - 2;
                    count = tt[len - date_index_end -1]->range_percent - tt[k]->range_percent;

                    index.insert(date_index);
                    index.insert(date_index_end);

                    if (count <= hi.fpoint)
                        search.append(name, ii->first, index);
                }
            }
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

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    count = 0;
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;


                    int k = len - date_index - 2;
                    count = tt[len - date_index_end -1]->range_percent - tt[k]->range_percent;

                    index.insert(date_index);
                    index.insert(date_index_end);

                    if (count <= hi.fpoint)
                        search.append(name, ii->first, index);
                }
            }
        }
    }


    return true;
}


bool hquotation_search_index::do_hq_sum_range_percent_ge(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    hidex_item hi;

    //-1:0:1.0&-2:0:1
    {
        std::vector<std::string> t_vec;
        SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 3) 
        { 
            return false;
        }

        hi.date_index = abs(atoi(t_vec[0].c_str()));
        get_first_not_numstr(t_vec[0], hi.start_str);
        hi.date_index_end = abs(atoi(t_vec[1].c_str()));
        get_first_not_numstr(t_vec[1], hi.end_str);


        hi.fpoint = atof(t_vec[2].c_str());
    }

    float count = 0;
    if (search.empty())
    {
        for (auto ii = search_index->id_sum_quotation.begin(); ii != search_index->id_sum_quotation.end(); ii++)
        {
            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    count = 0;
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;


                    int k = len - date_index - 2;
                    count = tt[len - date_index_end -1]->range_percent - tt[k]->range_percent;

                    index.insert(date_index);
                    index.insert(date_index_end);

                    if (count >= hi.fpoint)
                        search.append(name, ii->first, index);
                }
            }
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

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    count = 0;
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;


                    int k = len - date_index - 2;
                    count = tt[len - date_index_end -1]->range_percent - tt[k]->range_percent;

                    index.insert(date_index);
                    index.insert(date_index_end);

                    if (count >= hi.fpoint)
                        search.append(name, ii->first, index);
                }
            }
        }
    }

    return true;
}

bool hquotation_search_index::do_check_hqdown_pointer_ge(std::string &name, std::string &value, search_res & search)                                              
{
    hquotation_search_item * search_index = current();
    SETS_OP_TRPE tmp_ot;
    proc_data* p_data = proc_data::instance();

    hidex_item hi;

    //-1:1.0&-2:1
    {
        std::vector<std::string> t_vec;
        SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 2) 
        { 
            return false;
        }

        hi.date_index = abs(atoi(t_vec[0].c_str()));
        get_first_not_numstr(t_vec[0], hi.start_str);

        hi.fpoint = atof(t_vec[1].c_str());
    }

    if (search.empty())
    {
        for (auto ii = search_index->id_technical.begin(); ii != search_index->id_technical.end(); ii++)
        {
            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            for (auto date_index: date_index_set)
            {
                if (len  < 1 + date_index)
                    continue;

                if (tt[len - date_index - 1]->down_pointer >= hi.fpoint)
                    search.append(name, ii->first, date_index);
            }
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

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            for (auto date_index: date_index_set)
            {
                if (len  < 1 + date_index)
                    continue;

                if (tt[len - date_index - 1]->down_pointer >= hi.fpoint)
                    search.append(name, ii->first, date_index);
            }
        }
    }

    return true;
}

bool hquotation_search_index::do_check_hqdown_pointer_le(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    hidex_item hi;

    //-1:1.0&-2:1
    {
        std::vector<std::string> t_vec;
        SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 2) 
        { 
            return false;
        }

        hi.date_index = abs(atoi(t_vec[0].c_str()));
        get_first_not_numstr(t_vec[0], hi.start_str);
        hi.fpoint = atof(t_vec[1].c_str());
    }

    if (search.empty())
    {
        for (auto ii = search_index->id_technical.begin(); ii != search_index->id_technical.end(); ii++)
        {
            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            for (auto date_index: date_index_set)
            {
                if (len  < 1 + date_index)
                    continue;

                if (tt[len - date_index - 1]->down_pointer <= hi.fpoint)
                    search.append(name, ii->first, date_index);
            }
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

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            for (auto date_index: date_index_set)
            {
                if (len  < 1 + date_index)
                    continue;

                if (tt[len - date_index - 1]->down_pointer <= hi.fpoint)
                    search.append(name, ii->first, date_index);
            }
        }
    }

    return true;
}

bool hquotation_search_index::do_check_hqdown_pointer_ge_num_ge(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    hidex_item hi;

    //-5:0:2:1.0&-2:0:1:1
    {
        std::vector<std::string> t_vec;
        SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 4) 
        { 
            return false;
        }

        hi.date_index = abs(atoi(t_vec[0].c_str()));
        get_first_not_numstr(t_vec[0], hi.start_str);
        hi.date_index_end = abs(atoi(t_vec[1].c_str()));
        get_first_not_numstr(t_vec[1], hi.end_str);

        hi.sum = atoi(t_vec[2].c_str());
        hi.fpoint = atof(t_vec[3].c_str());
    }

    int count = 0;
    if (search.empty())
    {
        for (auto ii = search_index->id_technical.begin(); ii != search_index->id_technical.end(); ii++)
        {
            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    count = 0;
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;

                    for (int k = len - date_index - 1; k <= len - date_index_end - 1; k++) {
                        if (tt[k]->down_pointer >= hi.fpoint) {

                            count++;
                            index.insert(len - k - 1);
                        }
                    }

                    if (count >= hi.sum)
                        search.append(name, ii->first, index);
                }
            }
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

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    count = 0;
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;

                    for (int k = len - date_index - 1; k <= len - date_index_end - 1; k++) {
                        if (tt[k]->down_pointer >= hi.fpoint) {
                            count++;
                            index.insert(len - k - 1);
                        }
                    }

                    if (count >= hi.sum)
                        search.append(name, ii->first, index);
                }
            }
        }
    }


    return true;
}


bool hquotation_search_index::do_check_hqup_pointer_ge(std::string &name, std::string &value, search_res & search)                                              
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    hidex_item hi;

    //-1:1.0&-2:1
    {
        std::vector<std::string> t_vec;
        SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 2) 
        { 
            return false;
        }

        hi.date_index = abs(atoi(t_vec[0].c_str()));
        get_first_not_numstr(t_vec[0], hi.start_str);

        hi.fpoint = atof(t_vec[1].c_str());
    }

    if (search.empty())
    {
        for (auto ii = search_index->id_technical.begin(); ii != search_index->id_technical.end(); ii++)
        {
            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            for (auto date_index: date_index_set)
            {
                if (len  < 1 + date_index)
                    continue;

                if (tt[len - date_index - 1]->up_pointer >= hi.fpoint)
                    search.append(name, ii->first, date_index);
            }
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


            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            for (auto date_index: date_index_set)
            {
                if (len  < 1 + date_index)
                    continue;

                if (tt[len - date_index - 1]->up_pointer >= hi.fpoint)
                    search.append(name, ii->first, date_index);
            }
        }
    }

    return true;
}

bool hquotation_search_index::do_check_hqup_pointer_le(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    hidex_item hi;

    //-1:1.0&-2:1
    {
        std::vector<std::string> t_vec;
        SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 2) 
        { 
            return false;
        }

        hi.date_index = abs(atoi(t_vec[0].c_str()));
        get_first_not_numstr(t_vec[0], hi.start_str);

        hi.fpoint = atof(t_vec[1].c_str());
    }

    if (search.empty())
    {
        for (auto ii = search_index->id_technical.begin(); ii != search_index->id_technical.end(); ii++)
        {
            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            for (auto date_index: date_index_set)
            {
                if (len  < 1 + date_index)
                    continue;

                if (tt[len - date_index - 1]->up_pointer <= hi.fpoint)
                    search.append(name, ii->first, date_index);
            }
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

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            for (auto date_index: date_index_set)
            {
                if (len  < 1 + date_index)
                    continue;

                if (tt[len - date_index - 1]->up_pointer <= hi.fpoint)
                    search.append(name, ii->first, date_index);
            }
        }
    }

    return true;
}

bool hquotation_search_index::do_check_hqup_pointer_ge_num_ge(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    hidex_item hi;

    //-5:0:2:1.0&-2:0:1:1
    {
        std::vector<std::string> t_vec;
        SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 4) 
        { 
            return false;
        }

        hi.date_index = abs(atoi(t_vec[0].c_str()));
        get_first_not_numstr(t_vec[0], hi.start_str);

        hi.date_index_end = abs(atoi(t_vec[1].c_str()));
        get_first_not_numstr(t_vec[1], hi.end_str);

        hi.sum = atoi(t_vec[2].c_str());
        hi.fpoint = atof(t_vec[3].c_str());
    }

    int count = 0;
    if (search.empty())
    {
        for (auto ii = search_index->id_technical.begin(); ii != search_index->id_technical.end(); ii++)
        {
            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    count = 0;
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;

                    for (int k = len - date_index - 1; k <= len - date_index_end - 1; k++) {
                        if (tt[k]->up_pointer >= hi.fpoint) {
                            count++;
                            index.insert(len - k - 1);
                        }
                    }

                    if (count >= hi.sum)
                        search.append(name, ii->first, index);
                }
            }
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

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    count = 0;
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;

                    for (int k = len - date_index - 1; k <= len - date_index_end - 1; k++) {
                        if (tt[k]->up_pointer >= hi.fpoint) {
                            count++;
                            index.insert(len - k - 1);
                        }
                    }

                    if (count >= hi.sum)
                        search.append(name, ii->first, index);
                }
            }
        }
    }

    return true;
}

bool hquotation_search_index::do_check_hqend_start_ge(std::string &name, std::string &value, search_res & search)                                              
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    hidex_item hi;

    //-1:1.0&-2:1
    {
        std::vector<std::string> t_vec;
        SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 2) 
        { 
            return false;
        }

        hi.date_index = abs(atoi(t_vec[0].c_str()));
        get_first_not_numstr(t_vec[0], hi.start_str);

        hi.fpoint = atof(t_vec[1].c_str());
    }

    if (search.empty())
    {
        for (auto ii = search_index->id_technical.begin(); ii != search_index->id_technical.end(); ii++)
        {
            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            for (auto date_index: date_index_set)
            {
                if (len  < 1 + date_index)
                    continue;

                if (tt[len - date_index - 1]->end_start >= hi.fpoint)
                    search.append(name, ii->first, date_index);
            }
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

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            for (auto date_index: date_index_set)
            {
                if (len  < 1 + date_index)
                    continue;

                if (tt[len - date_index - 1]->end_start >= hi.fpoint)
                    search.append(name, ii->first, date_index);
            }
        }
    }

    return true;
}

bool hquotation_search_index::do_check_hqend_start_le(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    hidex_item hi;

    //-1:1.0&-2:1
    {
        std::vector<std::string> t_vec;
        SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 2) 
        { 
            return false;
        }

        hi.date_index = abs(atoi(t_vec[0].c_str()));
        get_first_not_numstr(t_vec[0], hi.start_str);
        hi.fpoint = atof(t_vec[1].c_str());
    }

    if (search.empty())
    {
        for (auto ii = search_index->id_technical.begin(); ii != search_index->id_technical.end(); ii++)
        {
            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            for (auto date_index: date_index_set)
            {
                if (len  < 1 + date_index)
                    continue;

                if (tt[len - date_index - 1]->end_start <= hi.fpoint)
                    search.append(name, ii->first, date_index);
            }
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

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            for (auto date_index: date_index_set)
            {
                if (len  < 1 + date_index)
                    continue;

                if (tt[len - date_index - 1]->end_start <= hi.fpoint)
                    search.append(name, ii->first, date_index);
            }
        }
    }

    return true;
}

bool hquotation_search_index::do_check_hqend_start_ge_num_ge(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    hidex_item hi;

    //-5:0:2:1.0&-2:0:1:1
    {
        std::vector<std::string> t_vec;
        SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 4) 
        { 
            return false;
        }

        hi.date_index = abs(atoi(t_vec[0].c_str()));
        get_first_not_numstr(t_vec[0], hi.start_str);

        hi.date_index_end = abs(atoi(t_vec[1].c_str()));
        get_first_not_numstr(t_vec[1], hi.end_str);

        hi.sum = atoi(t_vec[2].c_str());
        hi.fpoint = atof(t_vec[3].c_str());
    }

    int count = 0;
    if (search.empty())
    {
        for (auto ii = search_index->id_technical.begin(); ii != search_index->id_technical.end(); ii++)
        {
            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    count = 0;
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;

                    for (int k = len - date_index - 1; k <= len - date_index_end - 1; k++) {
                        if (tt[k]->end_start >= hi.fpoint) {
                            count++;
                            index.insert(len - k - 1);
                        }
                    }

                    if (count >= hi.sum)
                        search.append(name, ii->first, index);
                }
            }
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

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    count = 0;
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;

                    for (int k = len - date_index - 1; k <= len - date_index_end - 1; k++) {
                        if (tt[k]->end_start >= hi.fpoint) {
                            count++;
                            index.insert(len - k - 1);
                        }
                    }

                    if (count >= hi.sum)
                        search.append(name, ii->first, index);
                }
            }
        }
    }

    return true;
}


bool hquotation_search_index::do_check_hqend_start_ge_ratio_ge(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    hidex_item hi;

    //-5:0:0.8:1.0&-2:0:0.8:1
    {
        std::vector<std::string> t_vec;
        SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 4) 
        { 
            return false;
        }

        hi.date_index = abs(atoi(t_vec[0].c_str()));
        get_first_not_numstr(t_vec[0], hi.start_str);

        hi.date_index_end = abs(atoi(t_vec[1].c_str()));
        get_first_not_numstr(t_vec[1], hi.end_str);

        hi.sum = atoi(t_vec[2].c_str());
        hi.fpoint = atof(t_vec[3].c_str());
    }

    int count = 0;
    if (search.empty())
    {
        for (auto ii = search_index->id_technical.begin(); ii != search_index->id_technical.end(); ii++)
        {
            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    count = 0;
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;

                    for (int k = len - date_index - 1; k <= len - date_index_end - 1; k++) {
                        if (tt[k]->end_start >= hi.fpoint) {
                            count++;
                            index.insert(len - k - 1);
                        }
                    }

                    if (count >= hi.sum * (date_index -  date_index_end + 1)) 
                        search.append(name, ii->first, index);
                }
            }
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


            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    count = 0;
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;

                    for (int k = len - date_index - 1; k <= len - date_index_end - 1; k++) {
                        if (tt[k]->end_start >= hi.fpoint) {
                            count++;
                            index.insert(len - k - 1);
                        }
                    }

                    if (count >= hi.sum * (date_index -  date_index_end + 1))
                        search.append(name, ii->first, index);
                }
            }
        }
    }

    return true;
}


bool hquotation_search_index::do_check_hqend_avg_end_ge(std::string &name, std::string &value, search_res & search)                                              
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    hidex_item hi;

    //-1:1.0&-2:1
    {
        std::vector<std::string> t_vec;
        SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 2) 
        { 
            return false;
        }

        hi.date_index = abs(atoi(t_vec[0].c_str()));
        get_first_not_numstr(t_vec[0], hi.start_str);

        hi.fpoint = atof(t_vec[1].c_str());
    }

    if (search.empty())
    {
        for (auto ii = search_index->id_technical.begin(); ii != search_index->id_technical.end(); ii++)
        {
            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            for (auto date_index: date_index_set)
            {
                if (len  < 1 + date_index)
                    continue;

                if (tt[len - date_index - 1]->end_avg_end >= hi.fpoint)
                    search.append(name, ii->first, date_index);
            }
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

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            for (auto date_index: date_index_set)
            {
                if (len  < 1 + date_index)
                    continue;

                if (tt[len - date_index - 1]->end_avg_end >= hi.fpoint)
                    search.append(name, ii->first, date_index);
            }
        }
    }

    return true;
}

bool hquotation_search_index::do_check_hqend_avg_end_le(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    hidex_item hi;

    //-1:1.0&-2:1
    {
        std::vector<std::string> t_vec;
        SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 2) 
        { 
            return false;
        }

        hi.date_index = abs(atoi(t_vec[0].c_str()));
        get_first_not_numstr(t_vec[0], hi.start_str);
        hi.fpoint = atof(t_vec[1].c_str());
    }

    if (search.empty())
    {
        for (auto ii = search_index->id_technical.begin(); ii != search_index->id_technical.end(); ii++)
        {
            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            for (auto date_index: date_index_set)
            {
                if (len  < 1 + date_index)
                    continue;

                if (tt[len - date_index - 1]->end_avg_end <= hi.fpoint)
                    search.append(name, ii->first, date_index);
            }
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

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            for (auto date_index: date_index_set)
            {
                if (len  < 1 + date_index)
                    continue;

                if (tt[len - date_index - 1]->end_avg_end <= hi.fpoint)
                    search.append(name, ii->first, date_index);
            }
        }
    }

    return true;
}

bool hquotation_search_index::do_check_hqend_avg_end_ge_num_ge(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    hidex_item hi;

    //-5:0:2:1.0&-2:0:1:1
    {
        std::vector<std::string> t_vec;
        SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 4) 
        { 
            return false;
        }

        hi.date_index = abs(atoi(t_vec[0].c_str()));
        get_first_not_numstr(t_vec[0], hi.start_str);

        hi.date_index_end = abs(atoi(t_vec[1].c_str()));
        get_first_not_numstr(t_vec[1], hi.end_str);

        hi.sum = atoi(t_vec[2].c_str());
        hi.fpoint = atof(t_vec[3].c_str());
    }

    int count = 0;
    if (search.empty())
    {
        for (auto ii = search_index->id_technical.begin(); ii != search_index->id_technical.end(); ii++)
        {
            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    count = 0;
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;

                    for (int k = len - date_index - 1; k <= len - date_index_end - 1; k++) {
                        if (tt[k]->end_avg_end >= hi.fpoint) {
                            count++;
                            index.insert(len - k - 1);
                        }
                    }

                    if (count >= hi.sum)
                        search.append(name, ii->first, index);
                }
            }
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

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    count = 0;
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;

                    for (int k = len - date_index - 1; k <= len - date_index_end - 1; k++) {
                        if (tt[k]->end_avg_end >= hi.fpoint) {
                            count++;
                            index.insert(len - k - 1);
                        }
                    }

                    if (count >= hi.sum)
                        search.append(name, ii->first, index);
                }
            }
        }
    }

    return true;
}

bool hquotation_search_index::do_check_id_substr(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    hidex_item hi;

    //300;60
    {
        hi.sstr = value;
    }

    if (search.empty())
    {
        for (auto ii = search_index->id_quotation.begin(); ii != search_index->id_quotation.end(); ii++)
        {
            const std::string & id = ii->first;
            {
                if (strstr(id.c_str(), hi.sstr.c_str()))
                    search.append(name, ii->first);
            }
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

            {
                if (strstr(id.c_str(), hi.sstr.c_str())) 
                    search.append(name, ii->first);
            }
        }
    }

    return true;
}

bool hquotation_search_index::do_check_rlow_hlow_ge(std::string &name, std::string &value, search_res & search)                                              
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    hidex_item hi;

    //-1:1.0&-2:1
    {
        std::vector<std::string> t_vec;
        SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 2) 
        { 
            return false;
        }

        hi.date_index = abs(atoi(t_vec[0].c_str()));
        get_first_not_numstr(t_vec[0], hi.start_str);

        hi.fpoint = atof(t_vec[1].c_str());
    }

    if (search.empty())
    {
        for (auto ii = search_index->id_quotation.begin(); ii != search_index->id_quotation.end(); ii++)
        {
            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            for (auto date_index: date_index_set)
            {
                if (len  < 1 + date_index)
                    continue;

                if (tt[len - 1]->low >= hi.fpoint * tt[len - date_index - 1]->low)
                    search.append(name, ii->first, date_index);
            }
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

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            for (auto date_index: date_index_set)
            {
                if (len  < 1 + date_index)
                    continue;

                if (tt[len - 1]->low >= hi.fpoint * tt[len - date_index - 1]->low)
                    search.append(name, ii->first, date_index);
            }
        }
    }

    return true;
}


bool hquotation_search_index::do_check_rlow_hlowest_ge(std::string &name, std::string &value, search_res & search)                                              
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    hidex_item hi;

    //-10:1:0:1.0:k1
    {
        std::vector<std::string> t_vec;
        SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 3) 
        { 
            return false;
        }

        hi.date_index = abs(atoi(t_vec[0].c_str()));
        get_first_not_numstr(t_vec[0], hi.start_str);

        hi.date_index_end = abs(atoi(t_vec[1].c_str()));
        get_first_not_numstr(t_vec[1], hi.end_str);

        hi.fpoint = atof(t_vec[2].c_str());
    }

    if (search.empty())
    {
        for (auto ii = search_index->id_quotation.begin(); ii != search_index->id_quotation.end(); ii++)
        {
            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};
            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;

                    p_data->get_search_sstr(ii->first, "lowest", hi.date_index, hi.date_index_end, search, index);
                    if (index.empty()) 
                        continue;

                    float min = tt[len - *index.begin() - 1]->low;                                        
                    if (tt[len - 1]->low >= hi.fpoint * min)
                        search.append(name, ii->first, index);
                }
            }
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

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};
            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;

                    p_data->get_search_sstr(ii->first, "lowest", hi.date_index, hi.date_index_end, search, index);
                    if (index.empty()) 
                        continue;

                    float min = tt[len - *index.begin() - 1]->low;                                        
                    if (tt[len - 1]->low >= hi.fpoint * min)
                        search.append(name, ii->first, index);
                }
            }
        }
    }

    return true;
}

bool hquotation_search_index::do_check_rlow_htrough_ge(std::string &name, std::string &value, search_res & search)                                              
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    hidex_item hi;

    //-10:1:0:1.0:k1
    {
        std::vector<std::string> t_vec;
        SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 3) 
        { 
            return false;
        }

        hi.date_index = abs(atoi(t_vec[0].c_str()));
        get_first_not_numstr(t_vec[0], hi.start_str);

        hi.date_index_end = abs(atoi(t_vec[1].c_str()));
        get_first_not_numstr(t_vec[1], hi.end_str);

        hi.fpoint = atof(t_vec[2].c_str());
    }

    if (search.empty())
    {
        for (auto ii = search_index->id_quotation.begin(); ii != search_index->id_quotation.end(); ii++)
        {
            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};
            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;

                    p_data->get_search_sstr(ii->first, "trough", hi.date_index, hi.date_index_end, search, index);
                    if (index.empty()) 
                        continue;

                    float min = tt[len - *index.begin() - 1]->low;                                        
                    if (tt[len - 1]->low >= hi.fpoint * min)
                        search.append(name, ii->first, index);
                }
            }
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

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};
            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;

                    p_data->get_search_sstr(ii->first, "trough", hi.date_index, hi.date_index_end, search, index);
                    if (index.empty()) 
                        continue;

                    float min = tt[len - *index.begin() - 1]->low;                                        
                    if (tt[len - 1]->low >= hi.fpoint * min)
                        search.append(name, ii->first, index);
                }
            }
        }
    }

    return true;
}

bool hquotation_search_index::do_check_rlow_hlowest_le(std::string &name, std::string &value, search_res & search)                                              
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    hidex_item hi;

    //-10:1:1.0;-5:1:1
    {
        std::vector<std::string> t_vec;
        SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 3) 
        { 
            return false;
        }

        hi.date_index = abs(atoi(t_vec[0].c_str()));
        get_first_not_numstr(t_vec[0], hi.start_str);
        hi.date_index_end = abs(atoi(t_vec[1].c_str()));
        get_first_not_numstr(t_vec[1], hi.end_str);


        hi.fpoint = atof(t_vec[2].c_str());
    }

    if (search.empty())
    {
        for (auto ii = search_index->id_quotation.begin(); ii != search_index->id_quotation.end(); ii++)
        {
            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};
            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;

                    p_data->get_search_sstr(ii->first, "lowest", hi.date_index, hi.date_index_end, search, index);
                    if (index.empty()) 
                        continue;

                    float min = tt[len - *index.begin() - 1]->low;                                        
                    if (tt[len - 1]->low <= hi.fpoint * min)
                        search.append(name, ii->first, index);
                }
            }
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

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};
            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;

                    p_data->get_search_sstr(ii->first, "lowest", hi.date_index, hi.date_index_end, search, index);
                    if (index.empty()) 
                        continue;

                    float min = tt[len - *index.begin() - 1]->low;                                        
                    if (tt[len - 1]->low <= hi.fpoint * min)
                        search.append(name, ii->first, index);
                }
            }
        }
    }

    return true;
}

bool hquotation_search_index::do_check_rlow_htrough_le(std::string &name, std::string &value, search_res & search)                                              
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    hidex_item hi;

    //-10:1:1.0;-5:1:1
    {
        std::vector<std::string> t_vec;
        SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 3) 
        { 
            return false;
        }

        hi.date_index = abs(atoi(t_vec[0].c_str()));
        get_first_not_numstr(t_vec[0], hi.start_str);
        hi.date_index_end = abs(atoi(t_vec[1].c_str()));
        get_first_not_numstr(t_vec[1], hi.end_str);


        hi.fpoint = atof(t_vec[2].c_str());
    }

    if (search.empty())
    {
        for (auto ii = search_index->id_quotation.begin(); ii != search_index->id_quotation.end(); ii++)
        {
            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};
            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;

                    p_data->get_search_sstr(ii->first, "trough", hi.date_index, hi.date_index_end, search, index);
                    if (index.empty()) 
                        continue;

                    float min = tt[len - *index.begin() - 1]->low;                                        
                    if (tt[len - 1]->low <= hi.fpoint * min)
                        search.append(name, ii->first, index);
                }
            }
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

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};
            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;

                    p_data->get_search_sstr(ii->first, "trough", hi.date_index, hi.date_index_end, search, index);
                    if (index.empty()) 
                        continue;

                    float min = tt[len - *index.begin() - 1]->low;                                        
                    if (tt[len - 1]->low <= hi.fpoint * min)
                        search.append(name, ii->first, index);
                }
            }
        }
    }

    return true;
}

bool hquotation_search_index::do_check_rhigh_hhighest_ge(std::string &name, std::string &value, search_res & search)                                              
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    hidex_item hi;

    //-10:1:1.0;-5:1:1
    {
        std::vector<std::string> t_vec;
        SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 3) 
        { 
            return false;
        }

        hi.date_index = abs(atoi(t_vec[0].c_str()));
        get_first_not_numstr(t_vec[0], hi.start_str);
        hi.date_index_end = abs(atoi(t_vec[1].c_str()));
        get_first_not_numstr(t_vec[1], hi.end_str);
        hi.fpoint = atof(t_vec[2].c_str());
    }

    if (search.empty())
    {
        for (auto ii = search_index->id_quotation.begin(); ii != search_index->id_quotation.end(); ii++)
        {
            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};
            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;

                    p_data->get_search_sstr(ii->first, "highest", hi.date_index, hi.date_index_end, search, index);
                    if (index.empty()) 
                        continue;

                    float min = tt[len - *index.begin() - 1]->low;                                        
                    if (tt[len - 1]->high >= hi.fpoint * min)
                        search.append(name, ii->first, index);
                }
            }
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

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};
            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;

                    p_data->get_search_sstr(ii->first, "highest", hi.date_index, hi.date_index_end, search, index);
                    if (index.empty()) 
                        continue;

                    float min = tt[len - *index.begin() - 1]->low;                                        
                    if (tt[len - 1]->high >= hi.fpoint * min)
                        search.append(name, ii->first, index);
                }
            }
        }
    }

    return true;
}

bool hquotation_search_index::do_check_rhigh_hcrest_ge(std::string &name, std::string &value, search_res & search)                                              
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    hidex_item hi;

    //-10:1:1.0;-5:1:1
    {
        std::vector<std::string> t_vec;
        SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 3) 
        { 
            return false;
        }

        hi.date_index = abs(atoi(t_vec[0].c_str()));
        get_first_not_numstr(t_vec[0], hi.start_str);
        hi.date_index_end = abs(atoi(t_vec[1].c_str()));
        get_first_not_numstr(t_vec[1], hi.end_str);
        hi.fpoint = atof(t_vec[2].c_str());
    }

    if (search.empty())
    {
        for (auto ii = search_index->id_quotation.begin(); ii != search_index->id_quotation.end(); ii++)
        {
            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};
            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;

                    p_data->get_search_sstr(ii->first, "crest", hi.date_index, hi.date_index_end, search, index);
                    if (index.empty()) 
                        continue;

                    float min = tt[len - *index.begin() - 1]->low;                                        
                    if (tt[len - 1]->high >= hi.fpoint * min)
                        search.append(name, ii->first, index);
                }
            }
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

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};
            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;

                    p_data->get_search_sstr(ii->first, "crest", hi.date_index, hi.date_index_end, search, index);
                    if (index.empty()) 
                        continue;

                    float min = tt[len - *index.begin() - 1]->low;                                        
                    if (tt[len - 1]->high >= hi.fpoint * min)
                        search.append(name, ii->first, index);
                }
            }
        }
    }

    return true;
}

bool hquotation_search_index::do_check_hqend_end_ge(std::string &name, std::string &value, search_res & search) 
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    hidex_item hi;

    //-10:1:1.0;-5:1:1
    {
        std::vector<std::string> t_vec;
        SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 3) 
        { 
            return false;
        }

        hi.date_index = abs(atoi(t_vec[0].c_str()));
        get_first_not_numstr(t_vec[0], hi.start_str);

        hi.date_index_end = abs(atoi(t_vec[1].c_str()));
        get_first_not_numstr(t_vec[1], hi.end_str);

        hi.fpoint = atof(t_vec[2].c_str());
    }

    if (search.empty())
    {
        for (auto ii = search_index->id_quotation.begin(); ii != search_index->id_quotation.end(); ii++)
        {
            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;

                    if (tt[len - date_index - 1]->end >= hi.fpoint * tt[len  -date_index_end- 1]->end) 
                    {
                        index.insert(date_index);
                        index.insert(date_index_end);
                        search.append(name, ii->first, index);
                    }
                }
            }
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

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;

                    if (tt[len - date_index - 1]->end >= hi.fpoint * tt[len  -date_index_end- 1]->end) 
                    {
                        index.insert(date_index);
                        index.insert(date_index_end);
                        search.append(name, ii->first, index);
                    }
                }
            }
        }
    }


    return true;
}


bool hquotation_search_index::do_check_hqend_end_le(std::string &name, std::string &value, search_res & search) 
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    hidex_item hi;

    //-10:1:1.0;-5:1:1
    {
        std::vector<std::string> t_vec;
        SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 3) 
        { 
            return false;
        }

        hi.date_index = abs(atoi(t_vec[0].c_str()));
        get_first_not_numstr(t_vec[0], hi.start_str);

        hi.date_index_end = abs(atoi(t_vec[1].c_str()));
        get_first_not_numstr(t_vec[1], hi.end_str);

        hi.fpoint = atof(t_vec[2].c_str());
    }

    if (search.empty())
    {
        for (auto ii = search_index->id_quotation.begin(); ii != search_index->id_quotation.end(); ii++)
        {
            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;

                    if (tt[len - date_index - 1]->end <= hi.fpoint * tt[len  -date_index_end- 1]->end)
                    {
                        index.insert(date_index);
                        index.insert(date_index_end);
                        search.append(name, ii->first, index);
                    }
                }
            }
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

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;

                    if (tt[len - date_index - 1]->end <= hi.fpoint * tt[len  -date_index_end- 1]->end)
                    {
                        index.insert(date_index);
                        index.insert(date_index_end);
                        search.append(name, ii->first, index);
                    }
                }
            }
        }
    }


    return true;
}

bool hquotation_search_index::do_check_hqswing_le(std::string &name, std::string &value, search_res & search) 
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    hidex_item hi;

    //-10:1:1.0;-5:1:1
    {
        std::vector<std::string> t_vec;
        SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 3) 
        { 
            return false;
        }

        hi.date_index = abs(atoi(t_vec[0].c_str()));
        get_first_not_numstr(t_vec[0], hi.start_str);

        hi.date_index_end = abs(atoi(t_vec[1].c_str()));
        get_first_not_numstr(t_vec[1], hi.end_str);

        hi.fpoint = atof(t_vec[2].c_str());
    }

    if (search.empty())
    {
        for (auto ii = search_index->id_quotation.begin(); ii != search_index->id_quotation.end(); ii++)
        {
            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;

                    float swing = abs(tt[len - date_index - 1]->end - tt[len  -date_index_end- 1]->end);
                    if (swing <= hi.fpoint * (tt[len - date_index - 1]->end < tt[len  -date_index_end- 1]->end ? tt[len - date_index - 1]->end: tt[len  -date_index_end- 1]->end))
                    {
                        index.insert(date_index);
                        index.insert(date_index_end);
                        search.append(name, ii->first, index);
                    }
                }
            }
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

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;

                    float swing = abs(tt[len - date_index - 1]->end - tt[len  -date_index_end- 1]->end);
                    if (swing <= hi.fpoint * (tt[len - date_index - 1]->end < tt[len  -date_index_end- 1]->end ? tt[len - date_index - 1]->end: tt[len  -date_index_end- 1]->end))
                    {
                        index.insert(date_index);
                        index.insert(date_index_end);
                        search.append(name, ii->first, index);
                    }
                }
            }
        }
    }


    return true;
}


bool hquotation_search_index::do_check_hqswing_ge(std::string &name, std::string &value, search_res & search) 
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    hidex_item hi;

    //-10:1:1.0;-5:1:1
    {
        std::vector<std::string> t_vec;
        SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 3) 
        { 
            return false;
        }

        hi.date_index = abs(atoi(t_vec[0].c_str()));
        get_first_not_numstr(t_vec[0], hi.start_str);

        hi.date_index_end = abs(atoi(t_vec[1].c_str()));
        get_first_not_numstr(t_vec[1], hi.end_str);

        hi.fpoint = atof(t_vec[2].c_str());
    }

    if (search.empty())
    {
        for (auto ii = search_index->id_quotation.begin(); ii != search_index->id_quotation.end(); ii++)
        {
            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;

                    float swing = abs(tt[len - date_index - 1]->end - tt[len  -date_index_end- 1]->end);
                    if (swing >= hi.fpoint * (tt[len - date_index - 1]->end < tt[len  -date_index_end- 1]->end ? tt[len - date_index - 1]->end: tt[len  -date_index_end- 1]->end))
                    {
                        index.insert(date_index);
                        index.insert(date_index_end);
                        search.append(name, ii->first, index);
                    }
                }
            }
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

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;

                    float swing = abs(tt[len - date_index - 1]->end - tt[len  -date_index_end- 1]->end);
                    if (swing >= hi.fpoint * (tt[len - date_index - 1]->end < tt[len  -date_index_end- 1]->end ? tt[len - date_index - 1]->end: tt[len  -date_index_end- 1]->end))
                    {
                        index.insert(date_index);
                        index.insert(date_index_end);
                        search.append(name, ii->first, index);
                    }
                }
            }
        }
    }


    return true;
}


bool hquotation_search_index::do_check_rhigh_hhighest_le(std::string &name, std::string &value, search_res & search)                                              
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    hidex_item hi;

    //-10:1:1.0;-5:1:1
    {
        std::vector<std::string> t_vec;
        SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 3) 
        { 
            return false;
        }

        hi.date_index = abs(atoi(t_vec[0].c_str()));
        get_first_not_numstr(t_vec[0], hi.start_str);
        hi.date_index_end = abs(atoi(t_vec[1].c_str()));
        get_first_not_numstr(t_vec[1], hi.end_str);
        hi.fpoint = atof(t_vec[2].c_str());
    }

    if (search.empty())
    {
        for (auto ii = search_index->id_quotation.begin(); ii != search_index->id_quotation.end(); ii++)
        {
            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};
            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;

                    p_data->get_search_sstr(ii->first, "highest", hi.date_index, hi.date_index_end, search, index);
                    if (index.empty()) 
                        continue;

                    float min = tt[len - *index.begin() - 1]->low;                                        
                    if (tt[len - 1]->high <= hi.fpoint * min)
                        search.append(name, ii->first, index);
                }
            }
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

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};
            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;

                    p_data->get_search_sstr(ii->first, "highest", hi.date_index, hi.date_index_end, search, index);
                    if (index.empty()) 
                        continue;

                    float max = tt[len - *index.begin() - 1]->low;                                        
                    if (tt[len - 1]->high <= hi.fpoint * max)
                        search.append(name, ii->first, index);
                }
            }
        }
    }

    return true;
}

bool hquotation_search_index::do_check_rhigh_hcrest_le(std::string &name, std::string &value, search_res & search)                                              
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    hidex_item hi;

    //-10:1:1.0;-5:1:1
    {
        std::vector<std::string> t_vec;
        SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 3) 
        { 
            return false;
        }

        hi.date_index = abs(atoi(t_vec[0].c_str()));
        get_first_not_numstr(t_vec[0], hi.start_str);
        hi.date_index_end = abs(atoi(t_vec[1].c_str()));
        get_first_not_numstr(t_vec[1], hi.end_str);
        hi.fpoint = atof(t_vec[2].c_str());
    }

    if (search.empty())
    {
        for (auto ii = search_index->id_quotation.begin(); ii != search_index->id_quotation.end(); ii++)
        {
            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};
            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;

                    p_data->get_search_sstr(ii->first, "crest", hi.date_index, hi.date_index_end, search, index);
                    if (index.empty()) 
                        continue;

                    float min = tt[len - *index.begin() - 1]->low;                                        
                    if (tt[len - 1]->high <= hi.fpoint * min)
                        search.append(name, ii->first, index);
                }
            }
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

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};
            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;

                    p_data->get_search_sstr(ii->first, "crest", hi.date_index, hi.date_index_end, search, index);
                    if (index.empty()) 
                        continue;

                    float max = tt[len - *index.begin() - 1]->low;                                        
                    if (tt[len - 1]->high <= hi.fpoint * max)
                        search.append(name, ii->first, index);
                }
            }
        }
    }

    return true;
}

bool hquotation_search_index::do_check_rlow_hlow_le(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    hidex_item hi;

    //-1:1.0&-2:1
    {
        std::vector<std::string> t_vec;
        SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 2) 
        { 
            return false;
        }

        hi.date_index = abs(atoi(t_vec[0].c_str()));
        get_first_not_numstr(t_vec[0], hi.start_str);
        hi.fpoint = atof(t_vec[1].c_str());
    }

    if (search.empty())
    {
        for (auto ii = search_index->id_quotation.begin(); ii != search_index->id_quotation.end(); ii++)
        {
            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            for (auto date_index: date_index_set)
            {
                if (len  < 1 + date_index)
                    continue;

                if (tt[len - 1]->low <= hi.fpoint * tt[len - date_index - 1]->low)
                    search.append(name, ii->first, date_index);
            }
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

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            for (auto date_index: date_index_set)
            {
                if (len  < 1 + date_index)
                    continue;

                if (tt[len - 1]->low <= hi.fpoint * tt[len - date_index - 1]->low)
                    search.append(name, ii->first, date_index);
            }
        }
    }

    return true;
}

bool hquotation_search_index::do_check_rlow_hlow_ge_num_ge(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    hidex_item hi;

    //-5:1:2:1.0&-2:1:1:1
    {
        std::vector<std::string> t_vec;
        SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 4) 
        { 
            return false;
        }

        hi.date_index = abs(atoi(t_vec[0].c_str()));
        get_first_not_numstr(t_vec[0], hi.start_str);
        hi.date_index_end = abs(atoi(t_vec[1].c_str()));
        get_first_not_numstr(t_vec[1], hi.end_str);
        hi.sum = atoi(t_vec[2].c_str());
        hi.fpoint = atof(t_vec[3].c_str());
    }

    int count = 0;
    if (search.empty())
    {
        for (auto ii = search_index->id_quotation.begin(); ii != search_index->id_quotation.end(); ii++)
        {
            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    count = 0;
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;

                    for (int k = len - date_index - 1; k <= len - date_index_end - 1; k++) {
                        if (tt[len -1]->low >= hi.fpoint * tt[k]->low) {
                            count++;
                            index.insert(len - k - 1);
                        }
                    }

                    if (count >= hi.sum)
                        search.append(name, ii->first, index);
                }
            }
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

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    count = 0;
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;

                    for (int k = len - date_index - 1; k <= len - date_index_end - 1; k++) {
                        if (tt[len -1]->low >= hi.fpoint * tt[k]->low) {
                            count++;
                            index.insert(len - k - 1);
                        }
                    }

                    if (count >= hi.sum)
                        search.append(name, ii->first, index);
                }
            }
        }
    }

    return true;
}


bool hquotation_search_index::do_check_rhigh_hhigh_ge(std::string &name, std::string &value, search_res & search)                                              
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    hidex_item hi;

    //-1:1.0&-2:1
    {
        std::vector<std::string> t_vec;
        SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 2) 
        { 
            return false;
        }

        hi.date_index = abs(atoi(t_vec[0].c_str()));
        get_first_not_numstr(t_vec[0], hi.start_str);
        hi.fpoint = atof(t_vec[1].c_str());
    }

    if (search.empty())
    {
        for (auto ii = search_index->id_quotation.begin(); ii != search_index->id_quotation.end(); ii++)
        {
            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            for (auto date_index: date_index_set)
            {
                if (len  < 1 + date_index)
                    continue;

                if (tt[len - 1]->high >= hi.fpoint * tt[len - date_index - 1]->high)
                    search.append(name, ii->first, date_index);
            }
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

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            for (auto date_index: date_index_set)
            {
                if (len  < 1 + date_index)
                    continue;

                if (tt[len - 1]->high >= hi.fpoint * tt[len - date_index - 1]->high)
                    search.append(name, ii->first, date_index);
            }
        }
    }


    return true;
}

bool hquotation_search_index::do_check_rhigh_hhigh_le(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    hidex_item hi;

    //-1:1.0&-2:1
    {
        std::vector<std::string> t_vec;
        SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 2) 
        { 
            return false;
        }

        hi.date_index = abs(atoi(t_vec[0].c_str()));
        get_first_not_numstr(t_vec[0], hi.start_str);

        hi.fpoint = atof(t_vec[1].c_str());
    }

    if (search.empty())
    {
        for (auto ii = search_index->id_quotation.begin(); ii != search_index->id_quotation.end(); ii++)
        {
            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            for (auto date_index: date_index_set)
            {
                if (len  < 1 + date_index)
                    continue;

                if (tt[len - 1]->high <= hi.fpoint * tt[len - date_index - 1]->high)
                    search.append(name, ii->first, date_index);
            }
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

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            for (auto date_index: date_index_set)
            {
                if (len  < 1 + date_index)
                    continue;

                if (tt[len - 1]->high <= hi.fpoint * tt[len - date_index - 1]->high)
                    search.append(name, ii->first, date_index);
            }
        }
    }


    return true;
}

bool hquotation_search_index::do_check_rhigh_hhigh_ge_num_ge(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    hidex_item hi;

    //-5:1:2:1.0&-2:1:1:1
    {
        std::vector<std::string> t_vec;
        SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 4) 
        { 
            return false;
        }

        hi.date_index = abs(atoi(t_vec[0].c_str()));
        get_first_not_numstr(t_vec[0], hi.start_str);
        hi.date_index_end = abs(atoi(t_vec[1].c_str()));
        get_first_not_numstr(t_vec[1], hi.end_str);
        hi.sum = atoi(t_vec[2].c_str());
        hi.fpoint = atof(t_vec[3].c_str());
    }

    int count = 0;
    if (search.empty())
    {
        for (auto ii = search_index->id_quotation.begin(); ii != search_index->id_quotation.end(); ii++)
        {
            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();


            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    count = 0;
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;

                    for (int k = len - date_index - 1; k <= len - date_index_end - 1; k++) {
                        if (tt[len -1]->high >= hi.fpoint * tt[k]->high) {
                            count++;
                            index.insert(len - k - 1);
                        }
                    }

                    if (count >= hi.sum)
                        search.append(name, ii->first, index);
                }
            }
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

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    count = 0;
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;

                    for (int k = len - date_index - 1; k <= len - date_index_end - 1; k++) {
                        if (tt[len -1]->high >= hi.fpoint * tt[k]->high) {
                            count++;
                            index.insert(len - k - 1);
                        }
                    }

                    if (count >= hi.sum)
                        search.append(name, ii->first, index);
                }
            }
        }
    }

    return true;
}

bool hquotation_search_index::do_check_hqend_end_5_ge(std::string &name, std::string &value, search_res & search)                                              
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    hidex_item hi;

    //-1:1.0&-2:1
    {
        std::vector<std::string> t_vec;
        SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 2) 
        { 
            return false;
        }

        hi.date_index = abs(atoi(t_vec[0].c_str()));
        get_first_not_numstr(t_vec[0], hi.start_str);
        hi.fpoint = atof(t_vec[1].c_str());
    }

    if (search.empty())
    {
        for (auto ii = search_index->id_technical.begin(); ii != search_index->id_technical.end(); ii++)
        {
            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            for (auto date_index: date_index_set)
            {
                if (len  < 1 + date_index)
                    continue;

                if (tt[len - date_index - 1]->end_end_5 >= hi.fpoint)
                    search.append(name, ii->first, date_index);
            }
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

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            for (auto date_index: date_index_set)
            {
                if (len  < 1 + date_index)
                    continue;

                if (tt[len - date_index - 1]->end_end_5 >= hi.fpoint)
                    search.append(name, ii->first, date_index);
            }
        }
    }

    return true;
}

bool hquotation_search_index::do_check_hqend_end_5_le(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    hidex_item hi;

    //-1:1.0&-2:1
    {
        std::vector<std::string> t_vec;
        SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 2) 
        { 
            return false;
        }

        hi.date_index = abs(atoi(t_vec[0].c_str()));
        get_first_not_numstr(t_vec[0], hi.start_str);
        hi.fpoint = atof(t_vec[1].c_str());
    }

    if (search.empty())
    {
        for (auto ii = search_index->id_technical.begin(); ii != search_index->id_technical.end(); ii++)
        {
            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            for (auto date_index: date_index_set)
            {
                if (len  < 1 + date_index)
                    continue;

                if (tt[len - date_index - 1]->end_end_5 <= hi.fpoint)
                    search.append(name, ii->first, date_index);
            }
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

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            for (auto date_index: date_index_set)
            {
                if (len  < 1 + date_index)
                    continue;

                if (tt[len - date_index - 1]->end_end_5 <= hi.fpoint)
                    search.append(name, ii->first, date_index);
            }
        }
    }

    return true;
}


bool hquotation_search_index::do_check_hqend_end_5_ge_num_ge(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    hidex_item hi;

    //-5:0:2:1.0&-2:0:1:1
    {
        std::vector<std::string> t_vec;
        SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 4) 
        { 
            return false;
        }

        hi.date_index = abs(atoi(t_vec[0].c_str()));
        get_first_not_numstr(t_vec[0], hi.start_str);
        hi.date_index_end = abs(atoi(t_vec[1].c_str()));
        get_first_not_numstr(t_vec[1], hi.end_str);
        hi.sum = atoi(t_vec[2].c_str());
        hi.fpoint = atof(t_vec[3].c_str());
    }

    int count = 0;
    if (search.empty())
    {
        for (auto ii = search_index->id_technical.begin(); ii != search_index->id_technical.end(); ii++)
        {
            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    count = 0;
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;

                    for (int k = len - date_index - 1; k <= len - date_index_end - 1; k++) {
                        if (tt[k]->end_end_5 >= hi.fpoint) {
                            count++;
                            index.insert(len - k - 1);
                        }
                    }

                    if (count >= hi.sum)
                        search.append(name, ii->first, index);
                }
            }
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

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    count = 0;
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;

                    for (int k = len - date_index - 1; k <= len - date_index_end - 1; k++) {
                        if (tt[k]->end_end_5 >= hi.fpoint) {
                            count++;
                            index.insert(len - k - 1);
                        }
                    }

                    if (count >= hi.sum)
                        search.append(name, ii->first, index);
                }
            }
        }
    }

    return true;
}


bool hquotation_search_index::do_check_hqend_end_10_ge(std::string &name, std::string &value, search_res & search)                                              
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    hidex_item hi;

    //-1:1.0&-2:1
    {
        std::vector<std::string> t_vec;
        SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 2) 
        { 
            return false;
        }

        hi.date_index = abs(atoi(t_vec[0].c_str()));
        get_first_not_numstr(t_vec[0], hi.start_str);
        hi.fpoint = atof(t_vec[1].c_str());
    }

    if (search.empty())
    {
        for (auto ii = search_index->id_technical.begin(); ii != search_index->id_technical.end(); ii++)
        {
            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            for (auto date_index: date_index_set)
            {
                if (len  < 1 + date_index)
                    continue;

                if (tt[len - date_index - 1]->end_end_10 >= hi.fpoint)
                    search.append(name, ii->first, date_index);
            }
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

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            for (auto date_index: date_index_set)
            {
                if (len  < 1 + date_index)
                    continue;

                if (tt[len - date_index - 1]->end_end_10 >= hi.fpoint)
                    search.append(name, ii->first, date_index);
            }
        }
    }

    return true;
}

bool hquotation_search_index::do_check_hqend_end_10_le(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    hidex_item hi;

    //-1:1.0&-2:1
    {
        std::vector<std::string> t_vec;
        SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 2) 
        { 
            return false;
        }

        hi.date_index = abs(atoi(t_vec[0].c_str()));
        get_first_not_numstr(t_vec[0], hi.start_str);
        hi.fpoint = atof(t_vec[1].c_str());
    }

    if (search.empty())
    {
        for (auto ii = search_index->id_technical.begin(); ii != search_index->id_technical.end(); ii++)
        {
            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            for (auto date_index: date_index_set)
            {
                if (len  < 1 + date_index)
                    continue;

                if (tt[len - date_index - 1]->end_end_10 <= hi.fpoint)
                    search.append(name, ii->first, date_index);
            }
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

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            for (auto date_index: date_index_set)
            {
                if (len  < 1 + date_index)
                    continue;

                if (tt[len - date_index - 1]->end_end_10 <= hi.fpoint)
                    search.append(name, ii->first, date_index);
            }
        }
    }

    return true;
}

bool hquotation_search_index::do_check_hqend_end_10_ge_num_ge(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    hidex_item hi;

    //-5:0:2:1.0&-2:0:1:1
    {
        std::vector<std::string> t_vec;
        SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 4) 
        { 
            return false;
        }

        hi.date_index = abs(atoi(t_vec[0].c_str()));
        get_first_not_numstr(t_vec[0], hi.start_str);
        hi.date_index_end = abs(atoi(t_vec[1].c_str()));
        get_first_not_numstr(t_vec[1], hi.end_str);
        hi.sum = atoi(t_vec[2].c_str());
        hi.fpoint = atof(t_vec[3].c_str());
    }

    int count = 0;
    if (search.empty())
    {
        for (auto ii = search_index->id_technical.begin(); ii != search_index->id_technical.end(); ii++)
        {
            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    count = 0;
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;

                    for (int k = len - date_index - 1; k <= len - date_index_end - 1; k++) {
                        if (tt[k]->end_end_10 >= hi.fpoint) {
                            count++;
                            index.insert(len - k - 1);
                        }
                    }

                    if (count >= hi.sum)
                        search.append(name, ii->first, index);
                }
            }
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

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    count = 0;
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;

                    for (int k = len - date_index - 1; k <= len - date_index_end - 1; k++) {
                        if (tt[k]->end_end_10 >= hi.fpoint) {
                            count++;
                            index.insert(len - k - 1);
                        }
                    }

                    if (count >= hi.sum)
                        search.append(name, ii->first, index);
                }
            }
        }
    }

    return true;
}

bool hquotation_search_index::do_check_hqend_end_20_ge(std::string &name, std::string &value, search_res & search)                                              
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    hidex_item hi;

    //-1:1.0&-2:1
    {
        std::vector<std::string> t_vec;
        SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 2) 
        { 
            return false;
        }

        hi.date_index = abs(atoi(t_vec[0].c_str()));
        get_first_not_numstr(t_vec[0], hi.start_str);
        hi.fpoint = atof(t_vec[1].c_str());
    }

    if (search.empty())
    {
        for (auto ii = search_index->id_technical.begin(); ii != search_index->id_technical.end(); ii++)
        {
            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            for (auto date_index: date_index_set)
            {
                if (len  < 1 + date_index)
                    continue;

                if (tt[len - date_index - 1]->end_end_20 >= hi.fpoint)
                    search.append(name, ii->first, date_index);
            }
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

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            for (auto date_index: date_index_set)
            {
                if (len  < 1 + date_index)
                    continue;

                if (tt[len - date_index - 1]->end_end_20 >= hi.fpoint)
                    search.append(name, ii->first, date_index);
            }
        }
    }

    return true;
}

bool hquotation_search_index::do_check_hqend_end_20_le(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    hidex_item hi;

    //-1:1.0&-2:1
    {
        std::vector<std::string> t_vec;
        SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 2) 
        { 
            return false;
        }

        hi.date_index = abs(atoi(t_vec[0].c_str()));
        get_first_not_numstr(t_vec[0], hi.start_str);
        hi.fpoint = atof(t_vec[1].c_str());
    }

    if (search.empty())
    {
        for (auto ii = search_index->id_technical.begin(); ii != search_index->id_technical.end(); ii++)
        {
            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            for (auto date_index: date_index_set)
            {
                if (len  < 1 + date_index)
                    continue;

                if (tt[len - date_index - 1]->end_end_20 <= hi.fpoint)
                    search.append(name, ii->first, date_index);
            }
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

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            for (auto date_index: date_index_set)
            {
                if (len  < 1 + date_index)
                    continue;

                if (tt[len - date_index - 1]->end_end_20 <= hi.fpoint)
                    search.append(name, ii->first, date_index);
            }
        }
    }

    return true;
}

bool hquotation_search_index::do_check_hqend_end_20_ge_num_ge(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    SETS_OP_TRPE tmp_ot;
    proc_data* p_data = proc_data::instance();

    hidex_item hi;

    //-5:0:2:1.0&-2:0:1:1
    {
        std::vector<std::string> t_vec;
        SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 4) 
        { 
            return false;
        }

        hi.date_index = abs(atoi(t_vec[0].c_str()));
        get_first_not_numstr(t_vec[0], hi.start_str);
        hi.date_index_end = abs(atoi(t_vec[1].c_str()));
        get_first_not_numstr(t_vec[1], hi.end_str);
        hi.sum = atoi(t_vec[2].c_str());
        hi.fpoint = atof(t_vec[3].c_str());
    }

    int count = 0;
    if (search.empty())
    {
        for (auto ii = search_index->id_technical.begin(); ii != search_index->id_technical.end(); ii++)
        {
            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    count = 0;
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;

                    for (int k = len - date_index - 1; k <= len - date_index_end - 1; k++) {
                        if (tt[k]->end_end_20 >= hi.fpoint) {
                            count++;
                            index.insert(len - k - 1);
                        }
                    }

                    if (count >= hi.sum)
                        search.append(name, ii->first, index);
                }
            }
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

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    count = 0;
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;

                    for (int k = len - date_index - 1; k <= len - date_index_end - 1; k++) {
                        if (tt[k]->end_end_20 >= hi.fpoint) {
                            count++;
                            index.insert(len - k - 1);
                        }
                    }

                    if (count >= hi.sum)
                        search.append(name, ii->first, index);
                }
            }
        }
    }

    return true;
}

bool hquotation_search_index::do_check_hqend_end_30_ge(std::string &name, std::string &value, search_res & search)                                              
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    hidex_item hi;

    //-1:1.0&-2:1
    {
        std::vector<std::string> t_vec;
        SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 2) 
        { 
            return false;
        }

        hi.date_index = abs(atoi(t_vec[0].c_str()));
        get_first_not_numstr(t_vec[0], hi.start_str);
        hi.fpoint = atof(t_vec[1].c_str());
    }

    if (search.empty())
    {
        for (auto ii = search_index->id_technical.begin(); ii != search_index->id_technical.end(); ii++)
        {
            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            for (auto date_index: date_index_set)
            {
                if (len  < 1 + date_index)
                    continue;

                if (tt[len - date_index - 1]->end_end_30 >= hi.fpoint)
                    search.append(name, ii->first, date_index);
            }
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

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            for (auto date_index: date_index_set)
            {
                if (len  < 1 + date_index)
                    continue;

                if (tt[len - date_index - 1]->end_end_30 >= hi.fpoint)
                    search.append(name, ii->first, date_index);
            }
        }
    }

    return true;
}

bool hquotation_search_index::do_check_hqend_end_30_le(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    hidex_item hi;

    //-1:1.0&-2:1
    {
        std::vector<std::string> t_vec;
        SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 2) 
        { 
            return false;
        }

        hi.date_index = abs(atoi(t_vec[0].c_str()));
        get_first_not_numstr(t_vec[0], hi.start_str);
        hi.fpoint = atof(t_vec[1].c_str());
    }

    if (search.empty())
    {
        for (auto ii = search_index->id_technical.begin(); ii != search_index->id_technical.end(); ii++)
        {
            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            for (auto date_index: date_index_set)
            {
                if (len  < 1 + date_index)
                    continue;

                if (tt[len - date_index - 1]->end_end_30 <= hi.fpoint)
                    search.append(name, ii->first, date_index);
            }
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

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            for (auto date_index: date_index_set)
            {
                if (len  < 1 + date_index)
                    continue;

                if (tt[len - date_index - 1]->end_end_30 <= hi.fpoint)
                    search.append(name, ii->first, date_index);
            }
        }
    }

    return true;
}

bool hquotation_search_index::do_check_hqend_end_30_ge_num_ge(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    hidex_item hi;

    //-5:0:2:1.0&-2:0:1:1
    {
        std::vector<std::string> t_vec;
        SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 4) 
        { 
            return false;
        }

        hi.date_index = abs(atoi(t_vec[0].c_str()));
        get_first_not_numstr(t_vec[0], hi.start_str);
        hi.date_index_end = abs(atoi(t_vec[1].c_str()));
        get_first_not_numstr(t_vec[1], hi.end_str);
        hi.sum = atoi(t_vec[2].c_str());
        hi.fpoint = atof(t_vec[3].c_str());
    }

    int count = 0;
    if (search.empty())
    {
        for (auto ii = search_index->id_technical.begin(); ii != search_index->id_technical.end(); ii++)
        {
            const std::deque< std::shared_ptr<technical_t>> &  tt = ii->second;
            int len = tt.size();

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    count = 0;
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;

                    for (int k = len - date_index - 1; k <= len - date_index_end - 1; k++) {
                        if (tt[k]->end_end_30 >= hi.fpoint) {
                            count++;
                            index.insert(len - k - 1);
                        }
                    }

                    if (count >= hi.sum)
                        search.append(name, ii->first, index);
                }
            }
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

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    count = 0;
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;

                    for (int k = len - date_index - 1; k <= len - date_index_end - 1; k++) {
                        if (tt[k]->end_end_30 >= hi.fpoint) {
                            count++;
                            index.insert(len - k - 1);
                        }
                    }

                    if (count >= hi.sum)
                        search.append(name, ii->first, index);
                }
            }
        }
    }

    return true;
}

bool hquotation_search_index::do_check_hqredvol_greenvol_ge(std::string &name, std::string &value, search_res & search)                                              
{
    hquotation_search_item * search_index = current();
    proc_data* p_data = proc_data::instance();

    hidex_item hi;

    //-1:0:1.0&-2:0:1
    {
        std::vector<std::string> t_vec;
        SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 3) 
        { 
            return false;
        }

        hi.date_index = abs(atoi(t_vec[0].c_str()));
        get_first_not_numstr(t_vec[0], hi.start_str);
        hi.date_index_end = abs(atoi(t_vec[1].c_str()));
        get_first_not_numstr(t_vec[1], hi.end_str);
        hi.fpoint = atof(t_vec[2].c_str());
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

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    red_change_rate = green_change_rate = 0;
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;

                    for (int k = len - date_index - 1; k <= len - date_index_end - 1; k++) {
                        if (tt[k]->end >= tt[k]->start)
                        {
                            red_change_rate += tt[k]->change_rate;
                        }
                        else
                        {
                            green_change_rate += tt[k]->change_rate;
                        }
                    }

                    if (red_change_rate >= green_change_rate * hi.fpoint) {
                        index.insert(date_index);
                        index.insert(date_index_end);
                        search.append(name, ii->first, index);
                    }
                }
            }
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
            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    red_change_rate = green_change_rate = 0;
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;

                    for (int k = len - date_index - 1; k <= len - date_index_end - 1; k++) {
                        if (tt[k]->end >= tt[k]->start)
                        {
                            red_change_rate += tt[k]->change_rate;
                        }
                        else
                        {
                            green_change_rate += tt[k]->change_rate;
                        }
                    }

                    if (red_change_rate >= green_change_rate * hi.fpoint) {
                        index.insert(date_index);
                        index.insert(date_index_end);
                        search.append(name, ii->first, index);
                    }
                }
            }
        }
    }

    return true;
}

bool hquotation_search_index::do_check_hqredvol_greenvol_le(std::string &name, std::string &value, search_res & search)
{
    hquotation_search_item * search_index = current();
    SETS_OP_TRPE tmp_ot;
    proc_data* p_data = proc_data::instance();

    hidex_item hi;

    //-1:0:1.0&-2:0:1
    {
        std::vector<std::string> t_vec;
        SplitString(value.c_str(), ':', &t_vec, SPLIT_MODE_ALL);
        if (t_vec.size() < 3) 
        { 
            return false;
        }

        hi.date_index = abs(atoi(t_vec[0].c_str()));
        get_first_not_numstr(t_vec[0], hi.start_str);
        hi.date_index_end = abs(atoi(t_vec[1].c_str()));
        get_first_not_numstr(t_vec[1], hi.end_str);
        hi.fpoint = atof(t_vec[2].c_str());
    }

    float red_change_rate, green_change_rate;
    red_change_rate = green_change_rate = 0;

    if (search.empty())
    {
        for (auto ii = search_index->id_quotation.begin(); ii != search_index->id_quotation.end(); ii++)
        {
            const std::deque< std::shared_ptr<quotation_t>> &  tt = ii->second;
            int len = tt.size();

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    red_change_rate = green_change_rate = 0;
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;

                    for (int k = len - date_index - 1; k <= len - date_index_end - 1; k++) {
                        if (tt[k]->end >= tt[k]->start)
                        {
                            red_change_rate += tt[k]->change_rate;
                        }
                        else
                        {
                            green_change_rate += tt[k]->change_rate;
                        }
                    }

                    if (red_change_rate <= green_change_rate * hi.fpoint) {
                        index.insert(date_index);
                        index.insert(date_index_end);
                        search.append(name, ii->first, index);
                    }
                }
            }
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

            std::set<int> date_index_set {hi.date_index};
            std::set<int> date_index_end_set {hi.date_index_end};

            if (!hi.start_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.start_str, hi.date_index, hi.date_index_end, search, date_index_set);
            }  

            if (!hi.end_str.empty())
            {    
                p_data->get_search_sstr(ii->first, hi.end_str, hi.date_index, hi.date_index_end, search, date_index_end_set);
            }  

            for (auto date_index: date_index_set)
            {
                for (auto date_index_end: date_index_end_set)
                {
                    red_change_rate = green_change_rate = 0;
                    std::set<int> index;
                    if (len  < 1 + date_index || date_index < date_index_end)
                        continue;

                    for (int k = len - date_index - 1; k <= len - date_index_end - 1; k++) {
                        if (tt[k]->end >= tt[k]->start)
                        {
                            red_change_rate += tt[k]->change_rate;
                        }
                        else
                        {
                            green_change_rate += tt[k]->change_rate;
                        }
                    }

                    if (red_change_rate <= green_change_rate * hi.fpoint) {
                        index.insert(date_index);
                        index.insert(date_index_end);
                        search.append(name, ii->first, index);
                    }
                }
            }
        }
    }

    return true;
}

