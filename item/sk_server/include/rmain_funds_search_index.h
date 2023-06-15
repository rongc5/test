#ifndef RMAIN_FUNDS_SEARCH_INDEX_H
#define RMAIN_FUNDS_SEARCH_INDEX_H
#include "sk_def.h"
#include "common_util.h"

class rmain_funds_search_index : public ToBufferMgr<rmain_funds_search_item>
{
public:
    rmain_funds_search_index();
};

#endif // RMAIN_FUNDS_SEARCH_INDEX_H
