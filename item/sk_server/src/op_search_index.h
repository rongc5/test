#ifndef __op_SEARCH_INDEX_H__
#define __op_SEARCH_INDEX_H__

#include "sk_def.h"
#include "common_util.h"

class op_search_index
{
    public:

        bool do_check_op_search(std::string &key, std::string &value, search_res & search);
};


#endif

