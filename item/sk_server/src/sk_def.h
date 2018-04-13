#ifndef __SK_DEF_H__
#define __SK_DEF_H__

#include "common_util.h"
#include "ul_sign.h"

#define MSG_HTTP_REQ 1002
#define MSG_HTTP_RES 1003


class http_req_msg
{
    public:

        http_cmd_type cmd_type;
        uint32_t time_out;
        std::string sid;
        std::string  url;
        std::string post_data;
        std::map<std::string, std::string> headers;

        http_req_msg()
        {
            time_out = 0;
        }
};


class http_res_msg: public normal_msg
{
    public:
        http_res_msg()
        {   
            _msg_op = MSG_HTTP_RES;
        }   

        virtual ~http_res_msg(){}

        std::string sid;
        std::string url;
        std::string response;
};

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

    uint32_t value;
    uint32_t cir_value;

    finance_t()
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

struct single_t
{
    std::deque<uint32_t> single_in;
    std::deque<uint32_t> single_out;
    std::deque<uint32_t> single_inflow;
};

struct real_single
{
    std::string id;
    std::vector<single_t> re_single;

    void reset()
    {
        re_single.clear();
        id.clear();
    }
};

struct last_single_t
{
    uint32_t single_in;
    float in_price;

    uint32_t single_out;
    float out_price;
};

struct history_single_t
{
    std::string id;
    std::map<std::string, std::vector<last_single_t> > date_single_map;
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

    float avg_price; 

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

        avg_price = 0;
    }
};

struct history_quotation_t
{
    std::string id;
    std::map<std::string, quotation_t> date_quotation_map;
};

struct str_hasher
{
    size_t operator()(const std::string & k) const
    {   
        uint64_t key = 0;

        create_sign_fs64(k.c_str(), k.length(), &key);

        return key;
    }   
};

#define TIMER_TYPE_RELOAD_CONF 10000
#define TIMER_TYPE_REAL_REQ 10001

#define TIMER_TYPE_HTTP_REQ 10002


#endif
