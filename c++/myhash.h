#ifndef BILIN_SWITCH_MYHASH_TABLE_H
#define BILIN_SWITCH_MYHASH_TABLE_H

#include "bilin_switch_mylist_head.h"

template<class KEY, class VALUE>
class switch_myhash_table
{
public:
    switch_myhash_table(int a = 100);
    virtual ~switch_myhash_table();
    int add_key_value(const KEY & k, const VALUE & v);
    int get_value(const KEY & k, VALUE & v);
    int earse_key_value(const KEY & k);
    void clear();
    void display();

   private:
    const int n_buckets_len;
    switch_mylist_head<KEY, VALUE> * m_list;
};

template<class KEY, class VALUE>
switch_myhash_table<KEY, VALUE>::switch_myhash_table(int a):n_buckets_len(a)
{
    m_list = new switch_mylist_head<KEY, VALUE>[n_buckets_len];
}

template<class KEY, class VALUE>
switch_myhash_table<KEY, VALUE>::~switch_myhash_table()
{
    delete []m_list;
}

template<class KEY, class VALUE>
int switch_myhash_table<KEY, VALUE>::add_key_value(const KEY & k, const VALUE & v)
{
    int index = k%n_buckets_len;

    return m_list[index].push_back_uniq(k, v);
}

template<class KEY, class VALUE>
int switch_myhash_table<KEY, VALUE>::get_value(const KEY & k, VALUE &v)
{
    int index = k%n_buckets_len;

    return m_list[index].find_value(k, v);
}

template<class KEY, class VALUE>
int switch_myhash_table<KEY, VALUE>::earse_key_value(const KEY &k)
{
    int index = k%n_buckets_len;

    return m_list[index].erase_uniq(k);
}

template<class KEY, class VALUE>
void switch_myhash_table<KEY, VALUE>::clear()
{
    for (int i = 0; i < n_buckets_len; ++i)
    {
        m_list[i].clear();
    }
}

template<class KEY, class VALUE>
void switch_myhash_table<KEY, VALUE>::display()
{
    for (int i = 0; i < n_buckets_len; ++i)
    {
        m_list[i].display();
    }
}


#endif // BILIN_SWITCH_MYHASH_TABLE_H

