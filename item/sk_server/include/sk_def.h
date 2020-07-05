#ifndef __SK_DEF_H__
#define __SK_DEF_H__

#include "common_util.h"

#include <stack>
#include "log_helper.h"

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
    //int in;
    //int out;
    int diff;

    single_t()
    {
        //in = 0;
        //out = 0;
        diff = 0;
    }

    bool operator != (const single_t & st) const;

    single_t & operator += (const single_t & st);
};

typedef std::vector<single_t> single_vec;

struct search_res
{
    //key_id, list
    std::unordered_map<std::string, std::unordered_map<std::string, std::set<int>, str_hasher>, str_hasher> _key_map;

    bool empty();

    bool exist_key_index(std::string & key);

    void get_index_bykey(const std::string &key, const std::string & id, std::set<int> & res);

    void earse_bykey(std::string & key);

    //c = A
    void assign(std::string & A, std::string &C);

    //c = a * B
    void get_intersection(std::string & A, std::string & B, std::string & C);

    //c = a + B
    void get_union(std::string & A, std::string & B, std::string & C);

    // C = A - B
    void get_diff(std::string & A, std::string & B, std::string & C);

    void append(std::string &key, const std::string &id);

    void append(std::string & key);

    void append(std::string &key, const std::string &id, int index);

    void append(std::string &key, const std::string &id, std::set<int> & index);

    void set_bykey(std::string &key);

    void get_bykey(std::string &key, std::set<std::string> & res);

    void clear_set();

    std::set<std::string> _id_sets;
};

using namespace std::placeholders;
typedef std::function<bool(std::string &key, std::string &value, search_res & search)> base_search_index;

typedef std::function<int(const std::string & id, int date_index, int date_index_end, std::set<int> & res)> search_sstr_index;

struct hsingle_search_item
{
    //id: vector<single_vec>
    std::unordered_map<std::string, std::deque<std::deque< std::shared_ptr<single_vec>>>, str_hasher> id_single;
    //id_index (vector -->index), date
    std::unordered_map<std::string, std::string, str_hasher> id_idx_date;
    //id_date, index
    std::unordered_map<std::string, int, str_hasher> id_date_idx;

    std::unordered_map<std::string, std::deque< std::shared_ptr<single_vec>>, str_hasher> id_sum_single;

    int get_index(const std::string & id, const std::string & date);

    std::string  get_date(const std::string & id, int index);

    void clear();

    static void creat_id_index_key(const std::string & id, int index, std::string & key);

    static void creat_id_date_key(const std::string & id, const std::string & date, std::string & key);
};


struct rsingle_search_item
{
    //id: vector<single_vec>
    std::unordered_map<std::string, std::deque< std::shared_ptr<single_vec>>, str_hasher> id_single;

    //reverse search;
    std::vector<std::multimap<int, std::string> > rsingle_diff_index;

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
    std::unordered_map<std::string, std::deque< std::shared_ptr<quotation_original>>, str_hasher> id_quotation;
    //id: vector<technical_t>
    std::unordered_map<std::string, std::deque< std::shared_ptr<technical_t>>, str_hasher> id_technical;

    //reverse search;
    std::multimap<float, std::string> end_index;
    std::multimap<float, std::string> change_rate_index;
    std::multimap<float, std::string> range_percent_index;
    std::multimap<float, std::string> down_pointer_index;
    std::multimap<float, std::string> up_pointer_index;
    std::multimap<float, std::string> end_avg_end_index;
    std::multimap<float, std::string> end_end5_index;
    std::multimap<float, std::string> end_end10_index;
    std::multimap<float, std::string> end_end20_index;
    //std::multimap<float, std::string> end_end30_index;

    void clear();
};

struct hquotation_search_item
{
    //id: vector<quotation_t>
    std::unordered_map<std::string, std::deque< std::shared_ptr<quotation_t>>, str_hasher> id_quotation;

    //id: vector<technical_t>
    std::unordered_map<std::string, std::deque< std::shared_ptr<technical_t>>, str_hasher> id_technical;

    //id_index (vector -->index), date
    std::unordered_map<std::string, std::string, str_hasher> id_idx_date;
    //id_date, index
    std::unordered_map<std::string, int, str_hasher> id_date_idx;

    //id: vector<quotation_t>
    std::unordered_map<std::string, std::deque< std::shared_ptr<quotation_t>>, str_hasher> id_sum_quotation;

    //id, vector<int> index vec, wave info
    std::unordered_map<std::string, std::set<int>, str_hasher> id_crest;

    std::unordered_map<std::string, std::set<int>, str_hasher> id_trough;

    int get_index(const std::string & id, const std::string & date);

    std::string  get_date(const std::string & id, int index);

    void clear();

    static void creat_id_index_key(const std::string & id, int index, std::string & key);

    static void creat_id_date_key(const std::string & id, const std::string & date, std::string & key);
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
    std::unordered_map<std::string, std::shared_ptr<finance_t>, str_hasher> id_finance;
    //reverse search; 
    std::multimap<int, std::string> pe_index;
    std::multimap<int, std::string> pb_index;
    std::multimap<int, std::string> value_index;
    std::multimap<int, std::string> cir_value_index;
    std::multimap<float, std::string> mgxj_index;
    std::multimap<float, std::string> mgsy_index;
    std::multimap<float, std::string> zysrgr_index;
    std::multimap<float, std::string> jlrgr_index;

    void clear();
};

struct lru_sstr_item
{
    //key, index in id history
    std::unordered_map<std::string, int, str_hasher> _mmap_index;
    //key, index in deque;
    std::unordered_map<std::string, std::list<std::string>::iterator, str_hasher> _mmap_deque;
    std::list<std::string> _dq;

    void clear();
};


class url_handler
{
    public:
        virtual ~url_handler(){}
        virtual void perform(http_req_head_para * req_head, std::string * recv_body, 
                http_res_head_para * res_head, std::string * send_body)=0;
};


enum SETS_OP_TRPE
{
    SETS_OP_INTERSECTION,
    SETS_OP_UNION
};

#define NORMAL_MSG_DESTROY_QT 1000
#define NORMAL_MSG_DESTROY_ST 1001

#define NORMAL_MSG_UPDATE_TRADE_DATE 1002

class destroy_msg: public normal_msg
{
    public:
        destroy_msg()
        {   
            //_msg_op = NORMAL_MSG_DESTROY;
        }   

        virtual ~destroy_msg(){}
        std::string id; 
};



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




#define LRU_SSR_LENGTH_DF 1024

std::shared_ptr<quotation_t> operator + (const std::shared_ptr<quotation_t>  qt, const std::shared_ptr<quotation_t>  mm);




#endif
