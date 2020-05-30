#ifndef __lruSsr_SEARCH_INDEX_H__
#define __lruSsr_SEARCH_INDEX_H__

#include "sk_def.h"
#include "common_util.h"

class lruSsr_search_index:public ToBufferMgr<lru_sstr_item>
{
    public:

        int get_search_sstr(const std::string & id, const std::string & sstr, int date_index, int date_index_end);

        void add_search_sstr(const std::string & id, const std::string & sstr, int date_index, int date_index_end, int index);

        void destroy_idle();

};


#endif

