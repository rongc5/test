#ifndef __SK_DEF_H__
#define __SK_DEF_H__

#include "common_util.h"


struct ban_t
{
    std::string id;
    std::string date;
};

struct single_t
{
    int in;
    int out;
    int diff;
    std::string price;

    single_t()
    {
        in = 0;
        out = 0;
        diff = 0;
    }

    bool operator != (const single_t & st) const
    {
        if (this->in != st.in || this->out != st.out || this->diff != st.diff || this->price != st.price)
        {
            return true;
        }

        return false;
    }
};

typedef std::vector<single_t> single_vec;

struct finance_t
{
    std::string id;
    std::string pe;
    std::string pb;
    std::string cir_value;
    std::string value;
    std::string mgsy;
    std::string mgxj;
    std::string mgsygr;
    std::string mgxjgr;
    std::string zysrgr;
    std::string yylrgr;
    std::string jlrgr;
    std::string time_str;
};

struct quotation_t
{
    std::string id;
    std::string name;

    std::string start;
    std::string end;
    std::string high;
    std::string low;
    std::string last_closed;

    std::string vol;
    std::string buy_vol;
    std::string sell_vol;

    std::string swing;

    std::string change_rate;
    std::string range_percent;

    std::string down_pointer;
    std::string up_pointer;
    std::string avg_price;
    std::string total_price;
};

#define NORMAL_MSG_DUMP 1000

#define TIMER_TYPE_RELOAD_CONF 10000
#define TIMER_TYPE_REQ_QUOTATION 10001
#define TIMER_TYPE_REQ_SINGLE 10002

#define TIMER_TYPE_QUOTATION_IDLE_2_CURRENT 10003
#define TIMER_TYPE_SINGLE_IDLE_2_CURRENT 10004

#define TIMER_TYPE_HTTP_REQ 10009


#define HTPP_RES_OK 1000
#define HTPP_RES_ERR 1001
#define HTPP_REQ_PATH_ERR 1002

#endif
