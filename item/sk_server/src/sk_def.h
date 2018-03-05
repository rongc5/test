#ifndef __SK_DEF_H__
#define __SK_DEF_H__

#include "common_util.h"

#define MSG_HTTP_REQ 1002
#define MSG_HTTP_RES 1003


class http_req_msg:public normal_msg
{
    public:
        http_req_msg()
        {
            _msg_op = MSG_HTTP_REQ;
        }
        virtual ~http_req_msg(){}

        http_cmd_type cmd_type;

        string sid;
        string  url;
        string post_data;
        map<string, string> headers;
};


class http_res_msg: public normal_msg
{
    public:
        http_res_msg()
        {   
            _msg_op = MSG_HTTP_RES;
        }   

        virtual ~http_res_msg(){}

        string sid;
        string url;
        string response;
};

struct finance_t
{
    string id;
    string date;

    float mgxj;
    float mgsy;

    float mgsygr;
    float mgxjgr;
    float zysrgr;
    float yylrgr;
    float jlrgr;

    finance_t()
    {
        mgxj = 0;
        mgsy = 0;

        mgsygr = 0;
        mgxjgr = 0;
        zysrgr = 0;
        yylrgr = 0;
        jlrgr = 0;
    }
};

struct single_t
{
    string id;

    deque<uint32_t> king_single_in;
    deque<uint32_t> king_single_out;
    deque<uint32_t> king_single_inflow;

    deque<uint32_t> large_single_in;
    deque<uint32_t> large_single_out;
    deque<uint32_t> large_single_inflow;

    deque<uint32_t> middle_single_in;
    deque<uint32_t> middle_single_out;
    deque<uint32_t> middle_single_inflow;
};

struct history_single_t
{
    string id;
    map<string, single_t> date_single_map;
};

struct quotation_t
{
    string id;
    string name;

    float start;
    float end;
    float high;
    float low;
    float last_closed;

    uint64_t vol;
    uint64_t sell_vol;
    uint64_t buy_vol;

    int pe;
    int pb;

    float swing;

    float change_rate;
    float range_percent;

    uint32_t value;
    uint32_t cir_value;

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

        pe = 0;
        pb = 0;

        swing = 0;
        change_rate = 0;
        range_percent = 0;

        value = 0;
        cir_value = 0;
    }
};





#endif
