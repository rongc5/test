#include "common_cfgparser.h"
#include "common_util.h"


common_cfgparser::common_cfgparser()
{
}

common_cfgparser::common_cfgparser(const std::string & file_name)
{
    load_cfg(file_name);
}

int common_cfgparser::load_cfg(const std::string & file_name)
{
    char buf[SIZE_LEN_2048];
    FILE *fp = fopen(file_name.c_str(), "r");
    if (fp == NULL)
    {
        return -1;
    }
    
    std::string section;
    std::map<std::string, std::string> option;
    while (fgets(buf, sizeof(buf), fp))
    {
        std::string tmp = trim(buf);
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
            std::vector<std::string> item;
            SplitString(tmp.c_str(), "=", &item, SPLIT_MODE_ONE);
            if (item.size() != 2)
            {
                continue;
            }

            option[trim(item[0].c_str())] = trim(item[1].c_str());
            entry[section] = option;
        }
    }

    fclose(fp);

    return 0;
}

int common_cfgparser::get_vale(const std::string & section, const std::string & option, std::string & value) const
{
    value.clear();

    std::map<std::string, std::map<std::string, std::string> >::const_iterator it;
    std::map<std::string, std::string>::const_iterator itt;

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

bool common_cfgparser::has_option(const std::string & section, const std::string & option) const
{
    std::map<std::string, std::map<std::string, std::string> >::const_iterator it;
    std::map<std::string, std::string>::const_iterator itt;

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

bool common_cfgparser::has_section(const std::string & section) const
{
    std::map<std::string, std::map<std::string, std::string> >::const_iterator it;
    it = entry.find(section);

    if (it != entry.end())
    {
        return true;
    }

    return false;
}

void common_cfgparser::get_options(const std::string & section, std::vector<std::string> & options) const
{
    std::map<std::string, std::map<std::string, std::string> >::const_iterator it;
    std::map<std::string, std::string>::const_iterator itt;

    it = entry.find(section);
    if (it != entry.end())
    {
        for (itt = it->second.begin(); itt != it->second.end(); itt++)
        {
            options.push_back(itt->first);
        }
    }
}

void common_cfgparser::get_sections(std::vector<std::string> & sections) const
{
    std::map<std::string, std::map<std::string, std::string> >::const_iterator it;
    for (it = entry.begin(); it != entry.end(); it++)
    {
        sections.push_back(it->first);
    }
}

void common_cfgparser::clear()
{
    entry.clear();
}
