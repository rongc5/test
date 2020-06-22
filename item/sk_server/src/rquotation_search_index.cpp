#include "rquotation_search_index.h"
#include "sk_util.h"
#include "proc_data.h"
#include "history_quotation_dict.h"

bool rquotation_search_index::do_check_end_le(std::string &name, std::string &value, search_res & search)
{
    float end = 0;
    std::multimap<float, std::string>::iterator it_le, it_ge, it;

    auto & search_index = current()->end_index;

    it_le = search_index.end(); 
    it_ge = search_index.begin();

    end = atof(value.c_str()); 
    it_le = search_index.upper_bound(end);

    for (it = it_ge; it != it_le; ++it)
    {
        if (search.empty()|| search._id_sets.count(it->second))
        {
            search.append(name, it->second);
        }
    }

    return true;
}

bool rquotation_search_index::do_check_end_ge(std::string &name, std::string &value, search_res & search)
{
    float end = 0;
    std::multimap<float, std::string>::iterator it_le, it_ge, it;

    auto & search_index = current()->end_index;
    it_le = search_index.end(); 
    it_ge = search_index.begin();

    end = atof(value.c_str()); 
    it_ge = search_index.lower_bound(end);

    for (it = it_ge; it != it_le; ++it)
    {
        if (search.empty()|| search._id_sets.count(it->second))
        {
            search.append(name, it->second);
        }
    }

    return true;
}            

bool rquotation_search_index::do_check_change_rate_le(std::string &name, std::string &value, search_res & search)
{
    float end = 0;
    std::multimap<float, std::string>::iterator it_le, it_ge, it;

    auto & search_index = current()->change_rate_index;

    it_le = search_index.end();
    it_ge = search_index.begin();

    end = atof(value.c_str());
    it_le = search_index.upper_bound(end);

    for (it = it_ge; it != it_le; ++it)
    {
        if (search.empty()|| search._id_sets.count(it->second))
        {
            search.append(name, it->second);
        }
    }

    return true;
}

bool rquotation_search_index::do_check_change_rate_ge(std::string &name, std::string &value, search_res & search)
{
    float end = 0;
    std::multimap<float, std::string>::iterator it_le, it_ge, it;

    auto &search_index = current()->change_rate_index;
    it_le = search_index.end();
    it_ge = search_index.begin();

    end = atof(value.c_str());
    it_ge = search_index.lower_bound(end);

    for (it = it_ge; it != it_le; ++it)
    {
        if (search.empty()|| search._id_sets.count(it->second))
        {
            search.append(name, it->second);
        }
    }

    return true;
}

bool rquotation_search_index::do_check_range_percent_le(std::string &name, std::string &value, search_res & search)
{
    float end = 0;
    std::multimap<float, std::string>::iterator it_le, it_ge, it; 

    auto & search_index = current()->range_percent_index;

    it_le = search_index.end();
    it_ge = search_index.begin();

    end = atof(value.c_str());
    it_le = search_index.upper_bound(end);

    for (it = it_ge; it != it_le; ++it)
    {   
        if (search.empty()|| search._id_sets.count(it->second))
        {
            search.append(name, it->second);
        }
    }   

    return true;
}

bool rquotation_search_index::do_check_range_percent_ge(std::string &name, std::string &value, search_res & search)
{
    float end = 0;
    std::multimap<float, std::string>::iterator it_le, it_ge, it; 

    auto & search_index = current()->range_percent_index;
    it_le = search_index.end();
    it_ge = search_index.begin();

    end = atof(value.c_str());
    it_ge = search_index.lower_bound(end);

    for (it = it_ge; it != it_le; ++it)
    {   
        if (search.empty()|| search._id_sets.count(it->second))
        {
            search.append(name, it->second);
        }
    }

    return true;
}

bool rquotation_search_index::do_check_down_pointer_le(std::string &name, std::string &value, search_res & search)
{
    float end = 0;
    std::multimap<float, std::string>::iterator it_le, it_ge, it; 

    auto & search_index = current()->down_pointer_index;

    it_le = search_index.end();
    it_ge = search_index.begin();

    end = atof(value.c_str());
    it_le = search_index.upper_bound(end);

    for (it = it_ge; it != it_le; ++it)
    {   
        if (search.empty()|| search._id_sets.count(it->second))
        {
            search.append(name, it->second);
        }
    }   

    return true;
}

bool rquotation_search_index::do_check_down_pointer_ge(std::string &name, std::string &value, search_res & search)
{
    float end = 0;
    std::multimap<float, std::string>::iterator it_le, it_ge, it; 

    auto & search_index = current()->down_pointer_index;
    it_le = search_index.end();
    it_ge = search_index.begin();

    end = atof(value.c_str());
    it_ge = search_index.lower_bound(end);

    for (it = it_ge; it != it_le; ++it)
    {   
        if (search.empty()|| search._id_sets.count(it->second))
        {
            search.append(name, it->second);
        }
    }

    return true;
}

bool rquotation_search_index::do_check_up_pointer_le(std::string &name, std::string &value, search_res & search)
{
    float end = 0;
    std::multimap<float, std::string>::iterator it_le, it_ge, it; 

    auto & search_index = current()->up_pointer_index;

    it_le = search_index.end();
    it_ge = search_index.begin();

    end = atof(value.c_str());
    it_le = search_index.upper_bound(end);

    for (it = it_ge; it != it_le; ++it)
    {   
        if (search.empty()|| search._id_sets.count(it->second))
        {
            search.append(name, it->second);
        }
    }   

    return true;
}

bool rquotation_search_index::do_check_up_pointer_ge(std::string &name, std::string &value, search_res & search)
{
    float end = 0;
    std::multimap<float, std::string>::iterator it_le, it_ge, it; 

    auto & search_index = current()->up_pointer_index;
    it_le = search_index.end();
    it_ge = search_index.begin();

    end = atof(value.c_str());
    it_ge = search_index.lower_bound(end);

    for (it = it_ge; it != it_le; ++it)
    {   
        if (search.empty()|| search._id_sets.count(it->second))
        {
            search.append(name, it->second);
        }
    }

    return true;
}

bool rquotation_search_index::do_check_end_avg_end_le(std::string &name, std::string &value, search_res & search)
{
    float end = 0;
    std::multimap<float, std::string>::iterator it_le, it_ge, it; 

    auto & search_index = current()->end_avg_end_index;

    it_le = search_index.end();
    it_ge = search_index.begin();

    end = atof(value.c_str());
    it_le = search_index.upper_bound(end);

    for (it = it_ge; it != it_le; ++it)
    {   
        if (search.empty()|| search._id_sets.count(it->second))
        {
            search.append(name, it->second);
        }
    }   

    return true;
}

bool rquotation_search_index::do_check_end_avg_end_ge(std::string &name, std::string &value, search_res & search)
{
    float end = 0;
    std::multimap<float, std::string>::iterator it_le, it_ge, it; 

    auto & search_index = current()->end_avg_end_index;
    it_le = search_index.end();
    it_ge = search_index.begin();

    end = atof(value.c_str());
    it_ge = search_index.lower_bound(end);

    for (it = it_ge; it != it_le; ++it)
    {   
        if (search.empty()|| search._id_sets.count(it->second))
        {
            search.append(name, it->second);
        }
    }

    return true;
}

bool rquotation_search_index::do_check_end_end5_le(std::string &name, std::string &value, search_res & search)
{
    float end = 0;
    std::multimap<float, std::string>::iterator it_le, it_ge, it; 

    auto &  search_index = current()->end_end5_index;

    it_le = search_index.end();
    it_ge = search_index.begin();

    end = atof(value.c_str());
    it_le = search_index.upper_bound(end);

    for (it = it_ge; it != it_le; ++it)
    {   
        if (search.empty()|| search._id_sets.count(it->second))
        {
            search.append(name, it->second);
        }
    }   

    return true;
}

bool rquotation_search_index::do_check_end_end5_ge(std::string &name, std::string &value, search_res & search)
{
    float end = 0;
    std::multimap<float, std::string>::iterator it_le, it_ge, it; 

    auto & search_index = current()->end_end5_index;
    it_le = search_index.end();
    it_ge = search_index.begin();

    end = atof(value.c_str());
    it_ge = search_index.lower_bound(end);

    for (it = it_ge; it != it_le; ++it)
    {   
        if (search.empty()|| search._id_sets.count(it->second))
        {
            search.append(name, it->second);
        }
    }

    return true;
}

bool rquotation_search_index::do_check_end_end10_le(std::string &name, std::string &value, search_res & search)
{
    float end = 0;
    std::multimap<float, std::string>::iterator it_le, it_ge, it; 

    auto & search_index = current()->end_end10_index;

    it_le = search_index.end();
    it_ge = search_index.begin();

    end = atof(value.c_str());
    it_le = search_index.upper_bound(end);

    for (it = it_ge; it != it_le; ++it)
    {   
        if (search.empty()|| search._id_sets.count(it->second))
        {
            search.append(name, it->second);
        }
    }   

    return true;
}

bool rquotation_search_index::do_check_end_end10_ge(std::string &name, std::string &value, search_res & search)
{
    float end = 0;
    std::multimap<float, std::string>::iterator it_le, it_ge, it; 

    auto & search_index = current()->end_end10_index;
    it_le = search_index.end();
    it_ge = search_index.begin();

    end = atof(value.c_str());
    it_ge = search_index.lower_bound(end);

    for (it = it_ge; it != it_le; ++it)
    {   
        if (search.empty()|| search._id_sets.count(it->second))
        {
            search.append(name, it->second);
        }
    }

    return true;
}

bool rquotation_search_index::do_check_end_end20_le(std::string &name, std::string &value, search_res & search)
{
    float end = 0;
    std::multimap<float, std::string>::iterator it_le, it_ge, it; 

    auto & search_index = current()->end_end20_index;

    it_le = search_index.end();
    it_ge = search_index.begin();

    end = atof(value.c_str());
    it_le = search_index.upper_bound(end);

    for (it = it_ge; it != it_le; ++it)
    {   
        if (search.empty()|| search._id_sets.count(it->second))
        {
            search.append(name, it->second);
        }
    }   

    return true;
}

bool rquotation_search_index::do_check_end_end20_ge(std::string &name, std::string &value, search_res & search)
{
    float end = 0;
    std::multimap<float, std::string>::iterator it_le, it_ge, it; 

    auto & search_index = current()->end_end20_index;
    it_le = search_index.end();
    it_ge = search_index.begin();

    end = atof(value.c_str());
    it_ge = search_index.lower_bound(end);

    for (it = it_ge; it != it_le; ++it)
    {   
        if (search.empty()|| search._id_sets.count(it->second))
        {
            search.append(name, it->second);
        }
    }

    return true;
}

/*
bool rquotation_search_index::do_check_end_end30_le(std::string &name, std::string &value, search_res & search)
{
    float end = 0;
    std::multimap<float, std::string>::iterator it_le, it_ge, it; 

    auto & search_index = current()->end_end30_index;

    it_le = search_index.end();
    it_ge = search_index.begin();

    end = atof(value.c_str());
    it_le = search_index.upper_bound(end);

    for (it = it_ge; it != it_le; ++it)
    {   
        if (search.empty()|| search._id_sets.count(it->second))
        {
            search.append(name, it->second);
        }
    }   

    return true;
}

bool rquotation_search_index::do_check_end_end30_ge(std::string &name, std::string &value, search_res & search)
{
    float end = 0;
    std::multimap<float, std::string>::iterator it_le, it_ge, it; 

    auto & search_index = current()->end_end30_index;
    it_le = search_index.end();
    it_ge = search_index.begin();

    end = atof(value.c_str());
    it_ge = search_index.lower_bound(end);

    for (it = it_ge; it != it_le; ++it)
    {   
        if (search.empty()|| search._id_sets.count(it->second))
        {
            search.append(name, it->second);
        }
    }

    return true;
}
*/
