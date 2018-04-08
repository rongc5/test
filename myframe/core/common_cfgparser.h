#ifndef __COMMON_CFGPARSER_H__
#define __COMMON_CFGPARSER_H__

#include "base_def.h"

class common_cfgparser
{
    public:
        common_cfgparser();

        explicit common_cfgparser(const std::string & file_name);

        int load_cfg(const std::string & file_name);

        int get_vale(const std::string & section, const std::string & option, std::string & value) const;

        bool has_option(const std::string & section, const std::string & option) const;

        bool has_section(const std::string & section) const;

        void get_options(const std::string & section, std::vector<std::string> & options) const;

        void get_sections(std::vector<std::string> & sections) const;

    private:
        std::map<std::string, std::map<std::string, std::string> > entry;
};


#endif
