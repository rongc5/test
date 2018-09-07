#ifndef __SK_DEF_H__
#define __SK_DEF_H__

#include "common_util.h"


struct ban_t
{
    char id[SIZE_LEN_10];
    char date[SIZE_LEN_10];

    ban_t()
    {
        id[0] = '\0';
        date[0] = '\0';
    }
};

struct single_t
{
    int in;
    int out;
    int diff;

    single_t()
    {
        in = 0;
        out = 0;
        diff = 0;
    }

    bool operator != (const single_t & st) const
    {
        if (this->in != st.in || this->out != st.out || this->diff != st.diff)
        {
            return true;
        }

        return false;
    }
};

typedef std::vector<single_t> single_vec;

struct finance_t
{
    char id[SIZE_LEN_10];
    int pe;
    int pb;
    int cir_value;
    int value;
    float mgsy;
    float mgxj;
    float mgsygr;
    float mgxjgr;
    float zysrgr;
    float yylrgr;
    float jlrgr;
};

struct quotation_t
{
    char id[SIZE_LEN_10];
    char name[SIZE_LEN_16];

    float start;
    float end;
    float high;
    float low;
    float last_closed;

    int vol;
    int buy_vol;
    int sell_vol;

    float swing;

    float change_rate;
    float range_percent;

    float down_pointer;
    float up_pointer;
    float avg_price;
    float total_price;

    quotation_t()
    {
        id[0] = '\0';
        name[0] = '\0';

        start = 0;
        end = 0;
        high = 0;
        low = 0;
        last_closed = 0;

        vol = 0;
        buy_vol = 0;
        sell_vol = 0;

        swing = 0;

        change_rate = 0;
        range_percent = 0;

        down_pointer = 0;
        up_pointer = 0;
        avg_price = 0;
        total_price = 0;
    }
};

#define NORMAL_MSG_DUMP 1000

#define TIMER_TYPE_RELOAD_CONF 10000
#define TIMER_TYPE_REQ_QUOTATION 10001
#define TIMER_TYPE_REQ_SINGLE 10002

#define TIMER_TYPE_QUOTATION_IDLE_2_CURRENT 10003
#define TIMER_TYPE_SINGLE_IDLE_2_CURRENT 10004

#define TIMER_TYPE_HTTP_REQ 10009
#define TIMER_TYPE_DESTROY_IDLE 10010


#define HTPP_RES_OK 1000
#define HTPP_RES_ERR 1001
#define HTPP_REQ_PATH_ERR 1002

#endif
