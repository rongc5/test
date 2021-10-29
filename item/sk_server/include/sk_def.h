#ifndef __SK_DEF_H__
#define __SK_DEF_H__

#include "common_util.h"

#include <stack>
#include "log_helper.h"
#include <curl/curl.h>

using namespace std;

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
    unsigned long long in;
    unsigned long long out;
    int diff;
    float dratio;

    single_t()
    {
        in = 0;
        out = 0;
        diff = 0;
        dratio = 0 ;
    }

    bool operator != (const single_t & st) const;

    single_t & operator += (const single_t & st);
};

struct main_funds_t
{
    long long force_in;
    long long force_out;
    long long force_diff;
    float force_dratio;
    long long small_in;
    long long small_out;
    long long small_diff;
    float small_dratio;
    long long all;

    main_funds_t()
    {
        force_in = 0;
        force_out = 0;
        small_in = 0;
        small_out = 0;
        force_diff = 0;
        small_diff = 0;
        force_dratio = 0;
        small_dratio = 0;
        all = 0;
    }
};

typedef vector<single_t> single_vec;

struct search_res
{
    //key_id, list
    unordered_map<string, unordered_map<string, set<int>, str_hasher>, str_hasher> _key_map;

    bool empty();

    bool exist_key_index(string & key);

    void get_index_bykey(const string &key, const string & id, set<int> & res);

    void earse_bykey(string & key);

    //c = A
    void assign(string & A, string &C);

    //c = a * B
    void get_intersection(string & A, string & B, string & C);

    //c = a + B
    void get_union(string & A, string & B, string & C);

    // C = A - B
    void get_diff(string & A, string & B, string & C);

    void append(string &key, const string &id);

    void append(string & key);

    void append(string &key, const string &id, int index);

    void append(string &key, const string &id, set<int> & index);

    void set_bykey(string &key);

    void get_bykey(string &key, set<string> & res);

    void clear_set();

    set<string> _id_sets;
};

using namespace placeholders;
typedef function<bool(string &key, string &value, search_res & search)> base_search_index;

typedef function<int(const string & id, int date_index, int date_index_end, set<int> & res)> search_sstr_index;

struct hsingle_search_item
{
    //id: vector<single_vec>
    unordered_map<string, deque<deque< shared_ptr<single_vec>>>, str_hasher> id_single;
    //id_index (vector -->index), date
    unordered_map<string, string, str_hasher> id_idx_date;
    //id_date, index
    unordered_map<string, int, str_hasher> id_date_idx;

    unordered_map<string, deque< shared_ptr<single_vec>>, str_hasher> id_sum_single;

    int get_index(const string & id, const string & date);

    string  get_date(const string & id, int index);

    void clear();

    static void creat_id_index_key(const string & id, int index, string & key);

    static void creat_id_date_key(const string & id, const string & date, string & key);
};


struct rsingle_search_item
{
    //id: vector<single_vec>
    unordered_map<string, deque< shared_ptr<single_vec>>, str_hasher> id_single;

    //reverse search;
    vector<multimap<int, string> > rsingle_diff_index;

    void clear();
};


struct quotation_t
{
    //char name[SIZE_LEN_16];

    float start;
    float end;
    float high;
    float low;
    //float last_closed;

    int vol;
    //int buy_vol;
    //int sell_vol;

    //float swing;

    float change_rate;
    float range_percent;

    float total_price;

    quotation_t & operator += (const quotation_t & qt);

    quotation_t();
};


struct quotation_original
{
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

    float total_price;
    float quantity_relative_ratio;

    quotation_original & operator += (const quotation_original & qt);

    quotation_original();
};

struct technical_t
{
    float end_5;
    float end_10;
    float end_20;
    //float end_30;
    //float end_60;
    float down_pointer;
    float up_pointer;
    float avg_end;
    float end_start;
    float end_avg_end;
    float end_end_5;
    float end_end_10;
    float end_end_20;
    //float end_end_30;
    //float end_end_60;
    float low_end_5;
    float high_end_5;

    technical_t();
};

struct rquotation_search_item
{
    //id: vector<quotation_t>
    unordered_map<string, deque< shared_ptr<quotation_original>>, str_hasher> id_quotation;
    //id: vector<technical_t>
    unordered_map<string, deque< shared_ptr<technical_t>>, str_hasher> id_technical;

    //reverse search;
    multimap<float, string> end_index;
    multimap<float, string> change_rate_index;
    multimap<float, string> range_percent_index;
    multimap<float, string> down_pointer_index;
    multimap<float, string> up_pointer_index;
    multimap<float, string> end_avg_end_index;
    multimap<float, string> end_end5_index;
    multimap<float, string> end_end10_index;
    multimap<float, string> end_end20_index;
    //multimap<float, string> end_end30_index;

    void clear();
};

struct hquotation_search_item
{
    //id: vector<quotation_t>
    unordered_map<string, deque< shared_ptr<quotation_t>>, str_hasher> id_quotation;

    //id: vector<technical_t>
    unordered_map<string, deque< shared_ptr<technical_t>>, str_hasher> id_technical;

    //id_index (vector -->index), date
    unordered_map<string, string, str_hasher> id_idx_date;
    //id_date, index
    unordered_map<string, int, str_hasher> id_date_idx;

    //id: vector<quotation_t>
    unordered_map<string, deque< shared_ptr<quotation_t>>, str_hasher> id_sum_quotation;

    //id, vector<int> index vec, wave info
    unordered_map<string, set<int>, str_hasher> id_crest;

    unordered_map<string, set<int>, str_hasher> id_trough;

    int get_index(const string & id, const string & date);

    string  get_date(const string & id, int index);

    void clear();

    static void creat_id_index_key(const string & id, int index, string & key);

    static void creat_id_date_key(const string & id, const string & date, string & key);
};

struct finance_t
{
    int pe;
    int pb;
    int cir_value;
    int value;
    float mgsy;
    float mgxj;
    float zysrgr;
    float jlrgr;

    finance_t()
    {
        pe = 0;
        pb = 0;
        cir_value = 0;
        value = 0;
        mgsy = 0;
        mgxj = 0;
        zysrgr = 0;
        jlrgr = 0;
    }
};

struct finance_search_item 
{
    //id, finance_t
    unordered_map<string, shared_ptr<finance_t>, str_hasher> id_finance;
    //reverse search; 
    multimap<int, string> pe_index;
    multimap<int, string> pb_index;
    multimap<int, string> value_index;
    multimap<int, string> cir_value_index;
    multimap<float, string> mgxj_index;
    multimap<float, string> mgsy_index;
    multimap<float, string> zysrgr_index;
    multimap<float, string> jlrgr_index;

    void clear();
};

struct lru_sstr_item
{
    //key, index in id history
    unordered_map<string, int, str_hasher> _mmap_index;
    //key, index in deque;
    unordered_map<string, list<string>::iterator, str_hasher> _mmap_deque;
    list<string> _dq;

    void clear();
};


class url_handler
{
    public:
        virtual ~url_handler(){}
        virtual void perform(http_req_head_para * req_head, string * recv_body, 
                http_res_head_para * res_head, string * send_body)=0;
};


enum SETS_OP_TRPE
{
    SETS_OP_INTERSECTION,
    SETS_OP_UNION
};

#define NORMAL_MSG_DESTROY_QT 1000
#define NORMAL_MSG_DESTROY_ST 1001

#define NORMAL_MSG_UPDATE_TRADE_DATE 1002
#define NORMAL_MSG_REG_FOR_DATE_INFO 1003
#define NORMAL_MSG_UPDATE_DATE_INFO 1004

#define NORMAL_MSG_REQ_QUOTATION 1007
#define NORMAL_MSG_REQ_SINGLE 1008


#define NORMAL_MSG_RESPONSE_QUOTATION 1009
#define NORMAL_MSG_RESPONSE_SINGLE 1010

#define NORMAL_MSG_QUOTATION_IDLE_2_CURRENT 1011
#define NORMAL_MSG_SINGLE_IDLE_2_CURRENT 1012

#define NORMAL_MSG_QUOTATION_DUMP 1013
#define NORMAL_MSG_SINGLE_DUMP 1014

#define NORMAL_MSG_QUOTATION_DICT_UPDATE 1015
#define NORMAL_MSG_SINGLE_DICT_UPDATE 1016

#define NORMAL_MSG_WQUOTATION_DICT_UPDATE 1017
#define NORMAL_MSG_WSINGLE_DICT_UPDATE 1018

#define NORMAL_MSG_HOLIDAY_DICT_UPDATE 1019


#define NORMAL_MSG_DESTROY_MF 1020
#define NORMAL_MSG_REQ_MAIN_FUNDS 1021
#define NORMAL_MSG_RESPONSE_MAIN_FUNDS 1022
#define NORMAL_MSG_MAIN_FUNDS_IDLE_2_CURRENT 1023
#define NORMAL_MSG_MAIN_FUNDS_DUMP 1024
#define NORMAL_MSG_MAIN_FUNDS_DICT_UPDATE 1025

class req_msg: public normal_msg
{
    public:
        req_msg(int op):normal_msg(op)
        {   
            _id_vec = nullptr;
        }   

        virtual ~req_msg(){}
        map<string, string> _headers;
        shared_ptr<set<string>> _id_vec;
};


class response_req_msg: public normal_msg
{
    public:
        response_req_msg(int op):normal_msg(op)
        {   
            _st = nullptr;
            _qt = nullptr;
            _mft = nullptr;
        }   

        virtual ~response_req_msg(){}
        string _id;
        shared_ptr<single_vec> _st;
        shared_ptr<quotation_original> _qt;
        shared_ptr<main_funds_t> _mft;
};


class destroy_msg: public normal_msg
{
    public:
        destroy_msg()
        {   
            //_msg_op = NORMAL_MSG_DESTROY;
        }   

        virtual ~destroy_msg(){}
        shared_ptr<set<string>>  id_vec; 
};

class reg_date_msg: public normal_msg
{
    public:
        reg_date_msg()
        {   
            //_msg_op = NORMAL_MSG_DESTROY;
        }   

        virtual ~reg_date_msg(){}
        uint32_t _thread_index;
};

class date_msg: public normal_msg
{
    public:
        date_msg()
        {   
            //_msg_op = NORMAL_MSG_DESTROY;
        }   

        virtual ~date_msg(){}
        string req_date; 
        string trade_date; 
};


#define TIMER_TYPE_RELOAD_CONF 10000
#define TIMER_TYPE_REQ_QUOTATION 10001
#define TIMER_TYPE_REQ_SINGLE 10002

#define TIMER_TYPE_QUOTATION_IDLE_2_CURRENT 10003
#define TIMER_TYPE_SINGLE_IDLE_2_CURRENT 10004


#define TIMER_TYPE_HTTP_REQ 10009
#define TIMER_TYPE_DESTROY_IDLE 10010


#define TIMER_TYPE_REQ_MAIN_FUNDS 10011
#define TIMER_TYPE_MAIN_FUNDS_IDLE_2_CURRENT 10012


#define HTPP_RES_OK 1000
#define HTPP_RES_ERR 1001
#define HTPP_REQ_PATH_ERR 1002




#define LRU_SSR_LENGTH_DF 1024

shared_ptr<quotation_t> operator + (const shared_ptr<quotation_t>  qt, const shared_ptr<quotation_t>  mm);




#endif
