#ifndef __SK_DEF_H__
#define __SK_DEF_H__

#include "common_util.h"

#include <stack>

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

    bool operator != (const single_t & st) const
    {
        //if (this->in != st.in || this->out != st.out || this->diff != st.diff)
        if (this->diff != st.diff)
        {
            return true;
        }

        return false;
    }

    single_t & operator += (const single_t & st)
    {
        //this->in += st.in;
        //this->out += st.out;
        this->diff += st.diff;

        return *this;
    }
};

typedef std::vector<single_t> single_vec;

struct search_res
{
    //key, id, list
    std::unordered_map<std::string, std::unordered_map<std::string, std::set<int>, str_hasher>, str_hasher> _key_map;
    bool empty()
    {
        return _id_sets.empty();
    }

    void earse_bykey(std::string & key)
    {
        _key_map.erase(key);
    }

    //c = a * B
    void get_intersection(std::string & A, std::string & B, std::string & C)
    {
        auto ia = _key_map.find(A);
        auto ib = _key_map.find(B);

        std::unordered_map<std::string, std::set<int>, str_hasher> mm;
        std::set<int> dq;

        if (ia == _key_map.end() || ib == _key_map.end())
        {
            _key_map[C] = mm;
            return;
        }

        for (auto ii = ia->second.begin(); ii != ia->second.end(); ii++)
        {
            if (ib->second.count(ii->first))
            {
                mm[ii->first] = dq;
            }
        }

        _key_map[C] = mm;
    }

    //c = a + B
    void get_union(std::string & A, std::string & B, std::string & C)
    {
        auto ia = _key_map.find(A);
        auto ib = _key_map.find(B);

        std::unordered_map<std::string, std::set<int>, str_hasher> mm;
        std::set<int> dq;

        if (ia == _key_map.end() && ib == _key_map.end())
        {
            _key_map[C] = mm;
            return;
        }

        if (ia != _key_map.end())
        {
            for (auto ii = ia->second.begin(); ii != ia->second.end(); ii++)
            {
                mm[ii->first] = dq; 
            }
        }

        if (ib != _key_map.end())
        {
            for (auto ii = ib->second.begin(); ii != ib->second.end(); ii++)
            {
                mm[ii->first] = dq; 
            }
        }

        _key_map[C] = mm; 
    }

    // C = A - B
    void get_diff(std::string & A, std::string & B, std::string & C)
    {
        auto ia = _key_map.find(A);
        auto ib = _key_map.find(B);

        std::unordered_map<std::string, std::set<int>, str_hasher> mm;
        std::set<int> dq;
        if (ia == _key_map.end())
        {
            _key_map[C] = mm;
            return;
        }

        if (ib == _key_map.end())
        {
            _key_map[C] = _key_map[A];
            return;
        }

         for (auto ii = ia->second.begin(); ii != ia->second.end(); ii++)
         {
            if (!ib->second.count(ii->first))
            {
                mm[ii->first] = dq;
            }
         }

        _key_map[C] = mm;
    }

    void append(std::string &key, const std::string &id)
    {
        auto ii = _key_map.find(key);
        if (ii == _key_map.end())
        {
            std::unordered_map<std::string, std::set<int>, str_hasher> mm;
            std::set<int> dq;
            mm[id] = dq;
            _key_map[key] = mm;
        }
        else 
        {
            auto iii = ii->second.find(id);
            if (iii == ii->second.end())
            {
                std::set<int> dq;
                ii->second[id] = dq;

            }
        }
    }

    void append(std::string & key)
    {
        auto ii = _key_map.find(key);
        if (ii == _key_map.end())
        {
            std::unordered_map<std::string, std::set<int>, str_hasher> mm;
            _key_map[key] = mm;
        }
    }

    void append(std::string &key, const std::string &id, int index)
    {
        auto ii = _key_map.find(key);
        if (ii == _key_map.end())
        {
            std::unordered_map<std::string, std::set<int>, str_hasher> mm;
            std::set<int> dq;
            dq.insert(index);
            mm[id] = dq;
            _key_map[key] = mm;
        }
        else 
        {
            auto iii = ii->second.find(id);
            if (iii == ii->second.end())
            {
                std::set<int> dq;
                dq.insert(index);
                ii->second[id] = dq;

            }
            else
            {
                iii->second.insert(index);
            }
        }
    }

    void set_bykey(std::string &key)
    {
        std::set<std::string> tmp;
        _id_sets.swap(tmp);

        auto ii = _key_map.find(key); 
        if (ii == _key_map.end()) 
        {

            return;
        }

        for (auto iii = ii->second.begin(); iii != ii->second.end(); iii++)
        {
            _id_sets.insert(iii->first);
        }
    }

    void get_bykey(std::string &key, std::set<std::string> & res)
    {
        auto ii = _key_map.find(key); 
        if (ii == _key_map.end()) 
        {

            return;
        }

        for (auto iii = ii->second.begin(); iii != ii->second.end(); iii++)
        {
            res.insert(iii->first);
        }

    }

    void clear_set()
    {
        _id_sets.clear();
    }


    std::set<std::string> _id_sets;
};

using namespace std::placeholders;
typedef std::function<bool(std::string &key, std::string &value, search_res & search)> base_search_index;

typedef std::function<int(const std::string & id, int date_index, int date_index_end)> search_sstr_index;

struct hsingle_search_item
{
    //id: vector<single_vec>
    std::unordered_map<std::string, std::deque<std::deque< std::shared_ptr<single_vec>>>, str_hasher> id_single;
    //id_index (vector -->index), date
    std::unordered_map<std::string, std::string, str_hasher> id_idx_date;
    //id_date, index
    std::unordered_map<std::string, int, str_hasher> id_date_idx;

    std::unordered_map<std::string, std::deque< std::shared_ptr<single_vec>>, str_hasher> id_sum_single;

    int get_index(const std::string & id, const std::string & date)
    {
        std::string key;
        creat_id_date_key(id, date, key);
        
        auto mm = id_date_idx.find(key);
        if (mm == id_date_idx.end())
            return -1;
        return mm->second;
    }

    std::string  get_date(const std::string & id, int index)
    {
        std::string key;
        creat_id_index_key(id, index, key);

        auto mm = id_idx_date.find(key);
        if (mm == id_idx_date.end())
            return "";
        return mm->second;
    }

    void clear()
    {
        id_single.clear();
        id_idx_date.clear();
        id_date_idx.clear();
        id_sum_single.clear();
    }

    static void creat_id_index_key(const std::string & id, int index, std::string & key)
    {
        key.clear();
        char t_buf[SIZE_LEN_128] = {'\0'};
        snprintf(t_buf, sizeof(t_buf), "%s_%d", id.c_str(), index);
        key.append(t_buf);
    }

    static void creat_id_date_key(const std::string & id, const std::string & date, std::string & key)
    {
        key.clear();
        char t_buf[SIZE_LEN_128] = {'\0'};
        snprintf(t_buf, sizeof(t_buf), "%s_%s", id.c_str(), date.c_str());
        key.append(t_buf);
    }
};


struct rsingle_search_item
{
    //id: vector<single_vec>
    std::unordered_map<std::string, std::deque< std::shared_ptr<single_vec>>, str_hasher> id_single;

    //reverse search;
    std::vector<std::multimap<int, std::string> > rsingle_diff_index;

    void clear()
    {
        id_single.clear();
        rsingle_diff_index.clear();
    }

};


struct quotation_t
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

    quotation_t & operator += (const quotation_t & qt)
    {
        this->start += qt.start;
        this->end += qt.end;
        this->high += qt.high;
        this->low += qt.low;
        this->last_closed += qt.last_closed;
        this->vol += qt.vol;
        this->buy_vol += qt.buy_vol;
        this->sell_vol += qt.sell_vol;
        this->swing += qt.swing;
        this->change_rate += qt.change_rate;
        this->range_percent += qt.range_percent;
        this->total_price += qt.total_price;

        return *this;
    }



    quotation_t()
    {
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

        total_price = 0;
    }
};

struct technical_t
{
    float end_5;
    float end_10;
    float end_20;
    float end_30;
    float end_60;
    float down_pointer;
    float up_pointer;
    float avg_end;
    float end_start;
    float end_avg_end;
    float end_end_5;
    float end_end_10;
    float end_end_20;
    float end_end_30;
    float end_end_60;

    technical_t()
    {
        end_5 = 0;
        end_10 = 0;
        end_20 = 0;
        end_30 = 0;
        end_60 = 0;
        down_pointer = 0;
        up_pointer = 0;
        avg_end = 0;
        end_start = 0;
        end_avg_end = 0;

        end_end_5 = 0;
        end_end_10 = 0;
        end_end_20 = 0;
        end_end_30 = 0;
        end_end_60 = 0;
    }
};

struct rquotation_search_item
{
    //id: vector<quotation_t>
    std::unordered_map<std::string, std::deque< std::shared_ptr<quotation_t>>, str_hasher> id_quotation;
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
    std::multimap<float, std::string> end_end30_index;

    void clear()
    {
        id_quotation.clear();
        id_technical.clear();
        end_index.clear();
        change_rate_index.clear();
        range_percent_index.clear();
        down_pointer_index.clear();
        up_pointer_index.clear();
        end_avg_end_index.clear();
        end_end5_index.clear();
        end_end10_index.clear();
        end_end20_index.clear();
        end_end30_index.clear();

    }
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

    int get_index(const std::string & id, const std::string & date)
    {
        std::string key;
        creat_id_date_key(id, date, key);
        
        auto mm = id_date_idx.find(key);
        if (mm == id_date_idx.end())
            return -1;
        return mm->second;
    }

    std::string  get_date(const std::string & id, int index)
    {
        std::string key;
        creat_id_index_key(id, index, key);

        auto mm = id_idx_date.find(key);
        if (mm == id_idx_date.end())
            return "";
        return mm->second;
    }

    void clear()
    {
        id_quotation.clear();
        id_idx_date.clear();
        id_date_idx.clear();
        id_sum_quotation.clear();
        id_technical.clear();
    }

    static void creat_id_index_key(const std::string & id, int index, std::string & key)
    {   
        key.clear();
        char t_buf[SIZE_LEN_128] = {'\0'};
        snprintf(t_buf, sizeof(t_buf), "%s_%d", id.c_str(), index);
        key.append(t_buf);
    }

    static void creat_id_date_key(const std::string & id, const std::string & date, std::string & key)
    {   
        key.clear();
        char t_buf[SIZE_LEN_128] = {'\0'};
        snprintf(t_buf, sizeof(t_buf), "%s_%s", id.c_str(), date.c_str());
        key.append(t_buf);
    } 
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

    void clear()
    {
        id_finance.clear();
        pe_index.clear();
        pb_index.clear();
        value_index.clear();
        cir_value_index.clear();
        mgxj_index.clear();
        mgsy_index.clear();
        zysrgr_index.clear();
        jlrgr_index.clear();
    }
};

struct lru_sstr_item
{
    //key, index in id history
    std::unordered_map<std::string, int, str_hasher> _mmap_index;
    //key, index in deque;
    std::unordered_map<std::string, std::list<std::string>::iterator, str_hasher> _mmap_deque;
    std::list<std::string> _dq;

    void clear()
    {
        _mmap_index.clear();
        _mmap_deque.clear();
        std::list<std::string> tq;
        _dq.swap(tq);
    }
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




#endif
