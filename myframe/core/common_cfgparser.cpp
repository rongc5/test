#include "common_cfgparser.h"
#include "common_util.h"


common_cfgparser::common_cfgparser()
{
}

common_cfgparser::common_cfgparser(const string & file_name)
{
    load_cfg(file_name);
}

int common_cfgparser::load_cfg(const string & file_name)
{
    char buf[SIZE_LEN_2048];
    FILE *fp = fopen(file_name.c_str(), "r");
    if (fp == NULL)
    {
        return -1;
    }
    
    string section;
    map<string, string> option;
    while (fgets(buf, sizeof(buf), fp))
    {
        string tmp = trim(buf);
        if (tmp.empty() || tmp[0] == '#')
        {
            continue;
        }

        if (strstr(tmp.c_str(), "[") && strstr(tmp.c_str(), "]"))
        {
            section = trim(tmp.c_str(), " []");
            option.clear();
            continue;
        }

        if (strstr(tmp.c_str(), "="))
        {
            vector<string> item;
            SplitString(tmp.c_str(), "=", &item, SPLIT_MODE_ONE);
            if (item.size() != 2)
            {
                continue;
            }

            option[trim(item[0].c_str())] = trim(item[1].c_str());
            entry[section] = option;
        }
    }

    return 0;
}

int common_cfgparser::get_vale(const string & section, const string & option, string & value) const
{
    value.clear();

    map<string, map<string, string> >::const_iterator it;
    map<string, string>::const_iterator itt;

    it = entry.find(section);
    if (it != entry.end())
    {
        itt = it->second.find(option);
        if (itt != it->second.end())
        {
            value = itt->second;
            return 0;
        }
    }
    
    return -1;
}

bool common_cfgparser::has_option(const string & section, const string & option) const
{
    map<string, map<string, string> >::const_iterator it;
    map<string, string>::const_iterator itt;

    it = entry.find(section);
    if (it != entry.end())
    {
        itt = it->second.find(option);
        if (itt != it->second.end())
        {
            return true;
        }
    }
    
    return false;
}

bool common_cfgparser::has_section(const string & section) const
{
    map<string, map<string, string> >::const_iterator it;
    it = entry.find(section);

    if (it != entry.end())
    {
        return true;
    }

    return false;
}

void common_cfgparser::get_options(const string & section, vector<string> & options) const
{
    map<string, map<string, string> >::const_iterator it;
    map<string, string>::const_iterator itt;

    it = entry.find(section);
    if (it != entry.end())
    {
        for (itt = it->second.begin(); itt != it->second.end(); itt++)
        {
            options.push_back(itt->first);
        }
    }
}

void common_cfgparser::get_sections(vector<string> & sections) const
{
    map<string, map<string, string> >::const_iterator it;
    for (it = entry.begin(); it != entry.end(); it++)
    {
        sections.push_back(it->first);
    }
}
