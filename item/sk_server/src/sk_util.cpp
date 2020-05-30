#include "sk_util.h"
#include <algorithm>

void get_standard_id(std::string & id)
{
    if (start_with(id.c_str(), "sz") || strstr(id.c_str(), "sh"))
    {
        return;
    }
    
    std::string t_str;
    if (start_with(id.c_str(), "60"))
    {
        t_str.append("sh");
        t_str.append(id);

        id = t_str;
        return;
    }

    if (start_with(id.c_str(), "30") || start_with(id.c_str(), "00"))
    {
        t_str.append("sz");
        t_str.append(id);

        id = t_str;
        return;
    }
}

std::string float_2_str(float a, int decimal)
{
    char buf[SIZE_LEN_64];
    buf[0] = '\0';

    snprintf(buf, sizeof(buf), "%.*f", a, decimal);

    std::string str;

    str.append(buf);

    return str;
}

std::string int_2_str(int a)
{
    char buf[SIZE_LEN_64];
    buf[0] = '\0';

    snprintf(buf, sizeof(buf), "%d",  a);

    std::string str;
    str.append(buf);

    return str;
}

void get_union(std::vector<std::set<std::string> > & arr, std::set<std::string> &res)
{
    res.clear();
    std::set<std::string> search, tmp;
    auto ii = arr.begin();
    if (arr.size())
    {
        search = (*ii);
        ii++;
    }

    for (; ii != arr.end(); ii++)
    {
        tmp.clear();

        std::set_union(search.begin(), search.end(), ii->begin(), ii->end(), std::inserter(tmp,tmp.begin()));

        if (tmp.empty())
        {
            return;
        }

        search = tmp;
    }

    res = search;
}

void get_intersection(std::vector<std::set<std::string> > & arr, std::set<std::string> &res)
{
    res.clear();
    std::set<std::string> search, tmp;
    auto ii = arr.begin();
    if (arr.size())
    {
        search = (*ii);
        ii++;
    }

    for (; ii != arr.end(); ii++)
    {
        tmp.clear();

        std::set_intersection(search.begin(), search.end(), ii->begin(), ii->end(), std::inserter(tmp,tmp.begin()));
        if (tmp.empty())
        {
            return;
        }

        search = tmp;
    }

    res = search;
}

void get_first_not_numstr(std::string & src, std::string & dst)
{
    int i = 0;
    for (i = 0; i < (int)src.size(); i++)
    {
        if ((src[i] >= 'a' && src[i] <= 'z') || (src[i] >= 'A' && src[i] <= 'Z'))
            break;           
    }

    if ( i < (int)src.size())
        dst = src.substr(i, src.size() - i);
}


