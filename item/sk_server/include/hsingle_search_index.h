#ifndef __hsingle_SEARCH_INDEX_H__
#define __hsingle_SEARCH_INDEX_H__

#include "sk_def.h"
#include "common_util.h"

class hsingle_search_index:public ToBufferMgr<hsingle_search_item>
{
    public:
        virtual ~hsingle_search_index(){}

        bool do_hsingle_diff_le(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_hsingle_diff_ge(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_hsingle_diff_ge_num_ge(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_hsingle_sum_ge(std::string &key, std::string &value, std::set<std::string> & search);

        bool do_hsingle_sum_le(std::string &key, std::string &value, std::set<std::string> & search);

        struct hidex_item
        {
            uint32_t single_index;
            int date_index;
            int date_index_end;
            int single;
            int single_diff;
            int sum;
            hidex_item()
            {
                single_index = 0;
                date_index = 0;
                date_index_end = 0;
                single = 0;
                single_diff = 0;
                sum = 0;
            }
        };

};


#endif

