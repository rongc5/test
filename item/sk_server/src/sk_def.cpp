#include "sk_def.h"


bool single_t::operator != (const single_t & st) const
{
    //if (this->in != st.in || this->out != st.out || this->diff != st.diff)
    if (this->diff != st.diff)
    {
        return true;
    }

    return false;
}

single_t & single_t::operator += (const single_t & st)
{
    //this->in += st.in;
    //this->out += st.out;
    this->diff += st.diff;

    return *this;
}



bool search_res::empty()
{
    return _id_sets.empty();
}

bool search_res::exist_key_index(std::string & key)
{
    auto ia = _key_map.find(key);

    return ia != _key_map.end();
}

void search_res::get_index_bykey(const std::string &key, const std::string & id, std::set<int> & res)
{
    res.clear();
    auto ii = _key_map.find(key); 
    if (ii == _key_map.end()) 
    {
        return;
    }

    auto iii = ii->second.find(id);
    if (iii != ii->second.end())
    {
        res.insert(iii->second.begin(), iii->second.end());
    }

    //LOG_NOTICE("key:%s  size: %d", key.c_str(), _key_map[key].size());
}

void search_res::earse_bykey(std::string & key)
{
    _key_map.erase(key);
}

//c = A
void search_res::assign(std::string & A, std::string &C)
{
    auto ia = _key_map.find(A);
    if (ia != _key_map.end())
    {
        _key_map[C] = _key_map[A];
    }
}

//c = a * B
void search_res::get_intersection(std::string & A, std::string & B, std::string & C)
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
void search_res::get_union(std::string & A, std::string & B, std::string & C)
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
void search_res::get_diff(std::string & A, std::string & B, std::string & C)
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

void search_res::append(std::string &key, const std::string &id)
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
            ii->second.insert(std::make_pair(id, dq));

        }

    }

}

void search_res::append(std::string & key)
{
    auto ii = _key_map.find(key);
    if (ii == _key_map.end())
    {   
        std::unordered_map<std::string, std::set<int>, str_hasher> mm;
        _key_map[key] = mm;

    }
}

void search_res::append(std::string &key, const std::string &id, int index)
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
            ii->second.insert(std::make_pair(id, dq));

        }
        else
        {
            iii->second.insert(index);
        }

    }
}


void search_res::append(std::string &key, const std::string &id, std::set<int> & index)
{
    auto ii = _key_map.find(key);
    if (ii == _key_map.end())
    {
        std::unordered_map<std::string, std::set<int>, str_hasher> mm;
        mm[id] = index;
        _key_map[key] = mm;
    }
    else 
    {
        auto iii = ii->second.find(id);
        if (iii == ii->second.end())
        {
            ii->second.insert(std::make_pair(id, index));
        }
        else
        {
            iii->second.insert(index.begin(), index.end());
        }

    }
}

void search_res::set_bykey(std::string &key)
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

void search_res::get_bykey(std::string &key, std::set<std::string> & res)
{
    res.clear();
    auto ii = _key_map.find(key); 
    if (ii == _key_map.end()) 
    {

        return;
    }

    for (auto iii = ii->second.begin(); iii != ii->second.end(); iii++)
    {
        res.insert(iii->first);
    }

    //LOG_NOTICE("key:%s  size: %d", key.c_str(), _key_map[key].size());
}


void search_res::clear_set()
{
    _id_sets.clear();
}




int hsingle_search_item::get_index(const std::string & id, const std::string & date)
{
    std::string key;
    creat_id_date_key(id, date, key);

    auto mm = id_date_idx.find(key);
    if (mm == id_date_idx.end())
        return -1;
    return mm->second;
}

std::string  hsingle_search_item::get_date(const std::string & id, int index)
{
    std::string key;
    creat_id_index_key(id, index, key);

    auto mm = id_idx_date.find(key);
    if (mm == id_idx_date.end())
        return "";
    return mm->second;
}

void hsingle_search_item::clear()
{
    id_single.clear();
    id_idx_date.clear();
    id_date_idx.clear();
    id_sum_single.clear();
}

void hsingle_search_item::creat_id_index_key(const std::string & id, int index, std::string & key)
{
    key.clear();
    char t_buf[SIZE_LEN_128] = {'\0'};
    snprintf(t_buf, sizeof(t_buf), "%s_%d", id.c_str(), index);
    key.append(t_buf);
}

void hsingle_search_item::creat_id_date_key(const std::string & id, const std::string & date, std::string & key)
{
    key.clear();
    char t_buf[SIZE_LEN_128] = {'\0'};
    snprintf(t_buf, sizeof(t_buf), "%s_%s", id.c_str(), date.c_str());
    key.append(t_buf);
}



void rsingle_search_item::clear()
{
    id_single.clear();
    rsingle_diff_index.clear();
}




quotation_t & quotation_t::operator += (const quotation_t & qt)
{
    this->start += qt.start;
    this->end += qt.end;
    this->high += qt.high;
    this->low += qt.low;
    //this->last_closed += qt.last_closed;
    this->vol += qt.vol;
    //this->buy_vol += qt.buy_vol;
    //this->sell_vol += qt.sell_vol;
    //this->swing += qt.swing;
    this->change_rate += qt.change_rate;
    this->range_percent += qt.range_percent;
    this->total_price += qt.total_price;

    return *this;
}

quotation_t::quotation_t()
{
    //name[0] = '\0';

    start = 0;
    end = 0;
    high = 0;
    low = 0;
    //last_closed = 0;

    vol = 0;
    //buy_vol = 0;
    //sell_vol = 0;

    //swing = 0;

    change_rate = 0;
    range_percent = 0;

    total_price = 0;
}



quotation_original & quotation_original::operator += (const quotation_original & qt)
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



quotation_original::quotation_original()
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


technical_t::technical_t()
{
    end_5 = 0;
    end_10 = 0;
    end_20 = 0;
    //end_30 = 0;
    //end_60 = 0;
    down_pointer = 0;
    up_pointer = 0;
    avg_end = 0;
    end_start = 0;
    end_avg_end = 0;

    end_end_5 = 0;
    end_end_10 = 0;
    end_end_20 = 0;
    //end_end_30 = 0;
    //end_end_60 = 0;
    low_end_5 = 0;
    high_end_5 = 0;
}

void rquotation_search_item::clear()
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
    //end_end30_index.clear();

}

int hquotation_search_item::get_index(const std::string & id, const std::string & date)
{
    std::string key;
    creat_id_date_key(id, date, key);

    auto mm = id_date_idx.find(key);
    if (mm == id_date_idx.end())
        return -1;
    return mm->second;
}

std::string  hquotation_search_item::get_date(const std::string & id, int index)
{
    std::string key;
    creat_id_index_key(id, index, key);

    auto mm = id_idx_date.find(key);
    if (mm == id_idx_date.end())
        return "";
    return mm->second;
}

void hquotation_search_item::clear()
{
    id_quotation.clear();
    id_idx_date.clear();
    id_date_idx.clear();
    id_sum_quotation.clear();
    id_technical.clear();
    id_crest.clear();
    id_trough.clear();
}

void hquotation_search_item::creat_id_index_key(const std::string & id, int index, std::string & key)
{   
    key.clear();
    char t_buf[SIZE_LEN_128] = {'\0'};
    snprintf(t_buf, sizeof(t_buf), "%s_%d", id.c_str(), index);
    key.append(t_buf);
}

void hquotation_search_item::creat_id_date_key(const std::string & id, const std::string & date, std::string & key)
{   
    key.clear();
    char t_buf[SIZE_LEN_128] = {'\0'};
    snprintf(t_buf, sizeof(t_buf), "%s_%s", id.c_str(), date.c_str());
    key.append(t_buf);
} 


void finance_search_item::clear()
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

void lru_sstr_item::clear()
{
    _mmap_index.clear();
    _mmap_deque.clear();
    std::list<std::string> tq;
    _dq.swap(tq);
}

std::shared_ptr<quotation_t> operator + (const std::shared_ptr<quotation_t>  qt, const std::shared_ptr<quotation_t>  mm) 
{   
    std::shared_ptr<quotation_t> qq = std::make_shared<quotation_t>();

    qq->start = mm->start + qt->start;
    qq->end = mm->end + qt->end; 
    qq->high = mm->high + qt->high; 
    qq->low = mm->low + qt->low; 
    //qq->last_closed = mm->last_closed + qt->last_closed;
    qq->vol = mm->vol + qt->vol; 
    //qq->buy_vol = mm->buy_vol + qt->buy_vol;
    //qq->sell_vol = mm->sell_vol + qt->sell_vol;
    //qq->swing = mm->swing + qt->swing;
    qq->change_rate = mm->change_rate + qt->change_rate;
    qq->range_percent = mm->range_percent + qt->range_percent; 
    qq->total_price = mm->total_price + qt->total_price;

    return qq; 
}  
