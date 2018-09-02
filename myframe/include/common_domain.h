#ifndef __COMMON_DOMAIN_H_
#define __COMMON_DOMAIN_H_

#include "common_def.h"
#include "common_util.h"

class common_obj_container;
class common_domain
{
    public:
        common_domain(common_obj_container * net_container);

        std::vector<std::string> * get_domain(std::string & domain);

        void add_domain(std::string & domain, std::vector<std::string> & vip, uint64_t time_out=MAX_DOMAIN_TIMEOUT);

        void delete_domain(std::string & domain);

        void handle_timeout(std::shared_ptr<timer_msg> & t_msg);

    protected:
        std::unordered_map<std::string, std::vector<std::string>, str_hasher> _domain_cache;

        std::unordered_map<uint64_t, std::string> _timerid_domain_map;
        std::unordered_map<std::string, uint64_t> _domain_timerid_map;

        common_obj_container * _net_container;
};

#endif
