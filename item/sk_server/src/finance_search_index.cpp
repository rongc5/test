#include "finance_search_index.h"
#include "sk_util.h"
#include "proc_data.h"
#include "history_quotation_dict.h"

bool finance_search_index::do_check_pe_le(std::string &key, std::string &value, search_res & search)
{
    int end = 0;
    std::multimap<int, std::string>::iterator it_le, it_ge, it;

    auto & search_index = current()->pe_index;

    it_le = search_index.end();
    it_ge = search_index.begin();

    end = atoi(value.c_str());
    it_le = search_index.upper_bound(end);

    for (it = it_ge; it != it_le; ++it)
    {
        search._id_sets.insert(it->second);
    }

    return true;
}

bool finance_search_index::do_check_pe_ge(std::string &key, std::string &value, search_res & search)
{
    int end = 0;
    std::multimap<int, std::string>::iterator it_le, it_ge, it;

    auto & search_index = current()->pe_index;
    it_le = search_index.end();
    it_ge = search_index.begin();

    end = atoi(value.c_str());
    it_ge = search_index.lower_bound(end);

    for (it = it_ge; it != it_le; ++it)
    {
        search._id_sets.insert(it->second);
    }

    return true;
}                 

bool finance_search_index::do_check_pb_le(std::string &key, std::string &value, search_res & search)
{
    int end = 0;
    std::multimap<int, std::string>::iterator it_le, it_ge, it;

    auto & search_index = current()->pb_index;

    it_le = search_index.end();
    it_ge = search_index.begin();

    end = atoi(value.c_str());
    it_le = search_index.upper_bound(end);

    for (it = it_ge; it != it_le; ++it)
    {
        search._id_sets.insert(it->second);
    }

    return true;
}

bool finance_search_index::do_check_pb_ge(std::string &key, std::string &value, search_res & search)
{
    int end = 0;
    std::multimap<int, std::string>::iterator it_le, it_ge, it;

    auto & search_index = current()->pb_index;
    it_le = search_index.end();
    it_ge = search_index.begin();

    end = atoi(value.c_str());
    it_ge = search_index.lower_bound(end);

    for (it = it_ge; it != it_le; ++it)
    {
        search._id_sets.insert(it->second);
    }

    return true;
}

bool finance_search_index::do_check_value_le(std::string &key, std::string &value, search_res & search)
{
    int end = 0;
    std::multimap<int, std::string>::iterator it_le, it_ge, it;

    auto & search_index = current()->value_index;

    it_le = search_index.end();
    it_ge = search_index.begin();

    end = atoi(value.c_str());
    it_le = search_index.upper_bound(end);

    for (it = it_ge; it != it_le; ++it)
    {
        search._id_sets.insert(it->second);
    }

    return true;
}

bool finance_search_index::do_check_value_ge(std::string &key, std::string &value, search_res & search)
{
    int end = 0;
    std::multimap<int, std::string>::iterator it_le, it_ge, it;

    auto & search_index = current()->value_index;
    it_le = search_index.end();
    it_ge = search_index.begin();

    end = atoi(value.c_str());
    it_ge = search_index.lower_bound(end);

    for (it = it_ge; it != it_le; ++it)
    {
        search._id_sets.insert(it->second);
    }

    return true;
}


bool finance_search_index::do_check_cir_value_le(std::string &key, std::string &value, search_res & search)
{
    int end = 0;
    std::multimap<int, std::string>::iterator it_le, it_ge, it;

    auto & search_index = current()->cir_value_index;

    it_le = search_index.end();
    it_ge = search_index.begin();

    end = atoi(value.c_str());
    it_le = search_index.upper_bound(end);

    for (it = it_ge; it != it_le; ++it)
    {
        search._id_sets.insert(it->second);
    }

    return true;
}

bool finance_search_index::do_check_cir_value_ge(std::string &key, std::string &value, search_res & search)
{
    int end = 0;
    std::multimap<int, std::string>::iterator it_le, it_ge, it;

    auto & search_index = current()->cir_value_index;
    it_le = search_index.end();
    it_ge = search_index.begin();

    end = atoi(value.c_str());
    it_ge = search_index.lower_bound(end);

    for (it = it_ge; it != it_le; ++it)
    {
        search._id_sets.insert(it->second);
    }

    return true;
}

bool finance_search_index::do_check_mgxj_le(std::string &key, std::string &value, search_res & search)
{
    float end = 0;
    std::multimap<float, std::string>::iterator it_le, it_ge, it;

    auto & search_index = current()->mgxj_index;

    it_le = search_index.end();
    it_ge = search_index.begin();

    end = atof(value.c_str());
    it_le = search_index.upper_bound(end);

    for (it = it_ge; it != it_le; ++it)
    {
        search._id_sets.insert(it->second);
    }

    return true;
}

bool finance_search_index::do_check_mgxj_ge(std::string &key, std::string &value, search_res & search)
{
    float end = 0;
    std::multimap<float, std::string>::iterator it_le, it_ge, it;

    auto & search_index = current()->mgxj_index;
    it_le = search_index.end();
    it_ge = search_index.begin();

    end = atof(value.c_str());
    it_ge = search_index.lower_bound(end);

    for (it = it_ge; it != it_le; ++it)
    {
        search._id_sets.insert(it->second);
    }

    return true;
}

bool finance_search_index::do_check_mgsy_le(std::string &key, std::string &value, search_res & search)
{
    float end = 0;
    std::multimap<float, std::string>::iterator it_le, it_ge, it;

    auto & search_index = current()->mgsy_index;

    it_le = search_index.end();
    it_ge = search_index.begin();

    end = atof(value.c_str());
    it_le = search_index.upper_bound(end);

    for (it = it_ge; it != it_le; ++it)
    {
        search._id_sets.insert(it->second);
    }

    return true;
}

bool finance_search_index::do_check_mgsy_ge(std::string &key, std::string &value, search_res & search)
{
    float end = 0;
    std::multimap<float, std::string>::iterator it_le, it_ge, it;

    auto & search_index = current()->mgsy_index;
    it_le = search_index.end();
    it_ge = search_index.begin();

    end = atof(value.c_str());
    it_ge = search_index.lower_bound(end);

    for (it = it_ge; it != it_le; ++it)
    {
        search._id_sets.insert(it->second);
    }

    return true;
}

bool finance_search_index::do_check_zysrgr_le(std::string &key, std::string &value, search_res & search)
{
    float end = 0;
    std::multimap<float, std::string>::iterator it_le, it_ge, it;

    auto & search_index = current()->zysrgr_index;

    it_le = search_index.end();
    it_ge = search_index.begin();

    end = atof(value.c_str());
    it_le = search_index.upper_bound(end);

    for (it = it_ge; it != it_le; ++it)
    {
        search._id_sets.insert(it->second);
    }

    return true;
}

bool finance_search_index::do_check_zysrgr_ge(std::string &key, std::string &value, search_res & search)
{
    float end = 0;
    std::multimap<float, std::string>::iterator it_le, it_ge, it;

    auto & search_index = current()->zysrgr_index;
    it_le = search_index.end();
    it_ge = search_index.begin();

    end = atof(value.c_str());
    it_ge = search_index.lower_bound(end);

    for (it = it_ge; it != it_le; ++it)
    {
        search._id_sets.insert(it->second);
    }

    return true;
}

bool finance_search_index::do_check_jlrgr_le(std::string &key, std::string &value, search_res & search)
{
    float end = 0;
    std::multimap<float, std::string>::iterator it_le, it_ge, it;

    auto & search_index = current()->jlrgr_index;

    it_le = search_index.end();
    it_ge = search_index.begin();

    end = atof(value.c_str());
    it_le = search_index.upper_bound(end);

    for (it = it_ge; it != it_le; ++it)
    {
        search._id_sets.insert(it->second);
    }

    return true;
}

bool finance_search_index::do_check_jlrgr_ge(std::string &key, std::string &value, search_res & search)
{
    float end = 0;
    std::multimap<float, std::string>::iterator it_le, it_ge, it;

    auto & search_index = current()->jlrgr_index;
    it_le = search_index.end();
    it_ge = search_index.begin();

    end = atof(value.c_str());
    it_ge = search_index.lower_bound(end);

    for (it = it_ge; it != it_le; ++it)
    {
        search._id_sets.insert(it->second);
    }

    return true;
}

