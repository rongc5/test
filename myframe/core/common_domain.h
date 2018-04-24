#ifndef __COMMON_DOMAIN_H_
#define __COMMON_DOMAIN_H_

#include "common_def.h"

class common_obj_container;
class common_domain
{
    public:
        common_domain(common_obj_container * net_container);

        std::vector<std::string> & get_domain(std::string & domain);

        void add_domain(std::string & domain, std::vector<std::string> & vip);

        void delete_domain(std::string & domain);




    protected:
        std::map<std::string, std::vector<std::string> > _domain_cache;
        common_obj_container * _net_container;
};

#endif
