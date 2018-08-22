#include "sk_util.h"

void get_standard_id(std::string & id)
{
    if (strstr(id.c_str(), "sz") || strstr(id.c_str(), "sh"))
    {
        return;
    }
    
    std::string t_str;
    if (strstr(id.c_str(), "60"))
    {
        t_str.append("sh");
        t_str.append(id);

        id = t_str;
        return;
    }

    if (strstr(id.c_str(), "30") || strstr(id.c_str(), "00"))
    {
        t_str.append("sz");
        t_str.append(id);

        id = t_str;
        return;
    }
}
