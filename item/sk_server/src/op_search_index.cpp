#include "op_search_index.h"
#include "sk_util.h"
#include "proc_data.h"


bool op_search_index::do_check_op_search(std::string &key, std::string &value, search_res & search)
{
    std::vector<std::string> vec;
    int pos = 0;
    for (int i = 0 ; i < (int)value.size(); i++)
    {
        if (value[i] == '*' || value[i] == '+' || value[i] == '-' || value[i] == '(' || value[i] == ')')
        {
            std::string str;
            if (i - pos > 0)
            {
                str = value.substr(pos, i - pos);
                str = StringTrim(str);
                if (!str.empty())
                    vec.push_back(str);
            }
            vec.push_back(std::string(1, value[i]));

            pos = i + 1;
        }
    }

    if (pos != (int)value.size())
    {
        std::string str = value.substr(pos, (int)value.size() - pos);
        str = StringTrim(str);
        if (!str.empty())
            vec.push_back(str);
    }


    std::stack<std::string> name;
    std::stack<std::string> op;
    std::string tmp_name = "ai,Aguhaonana";

    for (int i = 0; i < (int)vec.size(); i++)
    {
        if (vec[i] == "(" || vec[i] == "*" || vec[i] == "+" || vec[i] == "-")
        {
            op.push(vec[i]);
        }
        else if (vec[i] == ")")
        {
            if (op.top() != "(")
                return false;
            op.pop();
        }
        else
        {
            if (name.empty())
            {
                name.push(vec[i]);
                continue;
            }

            if (op.top() == "*")
            {
                search.get_intersection(name.top(), vec[i], key);
            }
            else if (op.top() == "+")
            {
                search.get_union(name.top(), vec[i], key);
            }
            else if (op.top() == "-")
            {
                search.get_diff(name.top(), vec[i], key);
            }

            if (name.top() != key)
                search.earse_bykey(name.top());
            search.earse_bykey(vec[i]);                         

            op.pop();
            name.pop();

            name.push(key);
        }
    }



    return true;

}



