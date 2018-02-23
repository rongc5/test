#ifndef __COMMON_CFGPARSER_H__
#define __COMMON_CFGPARSER_H__

#include "base_def.h"

class common_cfgparser
{
    public:
        common_cfgparser();

        explicit common_cfgparser(const string & file_name);

        int load_cfg(const string & file_name);

        int get_vale(const string & section, const string & option, string & value) const;

        bool has_option(const string & section, const string & option) const;

        bool has_section(const string & section) const;

        void get_options(const string & section, vector<string> & options) const;

        void get_sections(vector<string> & sections) const;

    private:
        map<string, map<string, string> > entry;
};


#endif
