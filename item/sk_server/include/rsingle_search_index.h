#ifndef __rsingle_SEARCH_INDEX_H__
#define __rsingle_SEARCH_INDEX_H__

#include "sk_def.h"
#include "common_util.h"

class rsingle_search_index:public ToBufferMgr<rsingle_search_item>
{
    public:
        virtual ~rsingle_search_index(){}

        bool do_check_rsingle_le(std::string &key, std::string &value, search_res & search);

        bool do_check_rsingle_ge(std::string &key, std::string &value, search_res & search);

        bool do_check_rsingle_diff2_le(std::string &key, std::string &value, search_res & search);

        bool do_check_rsingle_diff2_ge(std::string &key, std::string &value, search_res & search);

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

