#ifndef __SK_DEF_H__
#define __SK_DEF_H__

#include "common_util.h"


struct finance_t
{
    std::string id;
    std::string time_str;

    float mgxj;
    float mgsy;

    float mgsygr;
    float mgxjgr;
    float zysrgr;
    float yylrgr;
    float jlrgr;

    int pe;
    int pb;

    int value;
    int cir_value;

    finance_t()
    {
        reset();
    }

    void reset()
    {
        mgxj = 0;
        mgsy = 0;

        mgsygr = 0;
        mgxjgr = 0;
        zysrgr = 0;
        yylrgr = 0;
        jlrgr = 0;

        pe = 0;
        pb = 0;

        value = 0;
        cir_value = 0;
    }
};

struct ban_t
{
    std::string id;
    std::string date;
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
    uint64_t sell_vol;
    uint64_t buy_vol;

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

struct history_quotation_t
{
    std::string id;
    std::map<std::string, quotation_t> date_quotation_map;
};


#define TIMER_TYPE_RELOAD_CONF 10000
#define TIMER_TYPE_REAL_REQ 10001

#define TIMER_TYPE_HTTP_REQ 10002


#endif
