#ifndef __SK_DEF_H__
#define __SK_DEF_H__

#include "common_util.h"


struct ban_t
{
    std::string id;
    std::string date;
};

struct history_single
{
    std::string single;
    std::string price;
};

struct quotation_t
{
    std::string id;
    std::string name;

    float start;
    float end;
    float high;
    float low;
    float last_closed;

    uint64_t vol;
    uint64_t buy_vol;
    uint64_t sell_vol;

    float swing;

    float change_rate;
    float range_percent;

    float total_price;
    bool blocked;

    quotation_t()
    {
        reset();
    }

    void reset()
    {
        id.clear();

        name.clear();

        start = 0;
        end = 0;
        high = 0;
        low = 0;
        last_closed = 0;

        vol = 0;
        sell_vol = 0;
        buy_vol = 0;

        swing = 0;
        change_rate = 0;
        range_percent = 0;
        blocked = false;
    }
};


struct single_index_vec
{
    std::vector<std::map<int, std::string> > single_vec;
};

#define TIMER_TYPE_RELOAD_CONF 10000
#define TIMER_TYPE_REQ_QUOTATION 10001
#define TIMER_TYPE_REQ_SINGLE 10002

#define TIMER_TYPE_HTTP_REQ 10002

#define HTPP_RES_OK 1000
#define HTPP_RES_ERR 1001
#define HTPP_REQ_PATH_ERR 1002

#endif
