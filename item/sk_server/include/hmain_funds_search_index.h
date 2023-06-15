#ifndef HMAIN_FUNDS_SEARCH_INDEX_H
#define HMAIN_FUNDS_SEARCH_INDEX_H
#include "sk_def.h"
#include "common_util.h"


class hmain_funds_search_index : public ToBufferMgr<hmain_funds_search_item>
{
public:
    hmain_funds_search_index();
};

#endif // HMAIN_FUNDS_SEARCH_INDEX_H
