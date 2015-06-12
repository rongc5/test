#ifndef BILIN_SWITCH_MYLIST_HEAD_H
#define BILIN_SWITCH_MYLIST_HEAD_H

#include <iostream>

template<class KEY, class VALUE>
struct list_head
{
public:
    list_head<KEY, VALUE>():is_valid(false){prev = next = this;}
    list_head<KEY, VALUE>(const KEY &k, const VALUE &v):is_valid(true), m_key(k), m_value(v){prev = next = this;}
    bool is_valid;
    list_head<KEY, VALUE> *prev, *next;
    KEY m_key;
    VALUE m_value;

private:
    list_head<KEY, VALUE>(const list_head<KEY, VALUE> &);
    list_head<KEY, VALUE>& operator=(const list_head<KEY, VALUE> &);
};

template<class KEY, class VALUE>
class switch_mylist_head
{
public:
    switch_mylist_head();
    bool is_empty();
    int push_back_uniq(const KEY & k, const VALUE & v);
    int push_back(const KEY & k, const VALUE & v);
    int find_value(const KEY & k, VALUE * v);
    int erase_uniq(const KEY & k);
    int erase(const KEY & k);
    int get_node_num();
    void display();

private:

    void __list_add(list_head<KEY, VALUE> * prev, list_head<KEY, VALUE> * node, list_head<KEY, VALUE> * next);

    int __add_tail(const KEY & k, const VALUE & v);

    int m_node_num;
    list_head<KEY, VALUE> m_list_entry;
    //CThreadMutex m_mutex;
};

template<class KEY, class VALUE>
switch_mylist_head<KEY, VALUE>::switch_mylist_head():m_node_num(0)
{
}

template<class KEY, class VALUE>
bool switch_mylist_head<KEY, VALUE>::is_empty()
{
    return !m_node_num;
}

template<class KEY, class VALUE>
void switch_mylist_head<KEY, VALUE>::__list_add(list_head<KEY, VALUE> * prev, list_head<KEY, VALUE> * node, list_head<KEY, VALUE> * next)
{
    prev->next = node;
    node->prev = prev;
    next->prev = node;
    node->next = next;
}

template<class KEY, class VALUE>
int switch_mylist_head<KEY, VALUE>::__add_tail(const KEY & k, const VALUE & v)
{
    list_head<KEY, VALUE> * ptr = new list_head<KEY, VALUE>(k, v);

    list_head<KEY, VALUE> * prev, *next;

    prev = m_list_entry.prev;
    next = &m_list_entry;

    __list_add(prev, ptr, next);

    return 0;
}

template<class KEY, class VALUE>
int switch_mylist_head<KEY, VALUE>::find_value(const KEY & k, VALUE * v)
{
    list_head<KEY, VALUE> * pos, *head;

    int flag = 0;

    //CThreadLock lock(&m_mutex);
    head = &m_list_entry;

     for (pos = head->next; pos != head; pos = pos->next)
    {
        if (pos->m_key == k)
        {
            *v = pos->m_value;
            flag = 1;
            break;
        }
    }

    return flag;
}

template<class KEY, class VALUE>
int switch_mylist_head<KEY, VALUE>::push_back_uniq(const KEY & k, const VALUE & v)
{
    VALUE tmp;
    int ret = 0;

    //CThreadLock lock(&m_mutex);

    ret = find_value(k, &tmp);
    if (ret)
        return 1;

    __add_tail(k, v);

     m_node_num++;

    return 0;
}


template<class KEY, class VALUE>
int switch_mylist_head<KEY, VALUE>::push_back(const KEY & k, const VALUE & v)
{
    //CThreadLock lock(&m_mutex);

    __add_tail(k, v);

     m_node_num++;

    return 0;
}

template<class KEY, class VALUE>
int switch_mylist_head<KEY, VALUE>::erase_uniq(const KEY & k)
{
    list_head<KEY, VALUE> * pos, *head, *n;

    //CThreadLock lock(&m_mutex);

    head = &m_list_entry;

    for (pos = head->next, n = pos ->next; pos != head; pos = n, n = n->next)
    {
        if (pos->m_key == k)
        {
            free(pos);
            m_node_num--;
            break;
        }
    }

    return 0;
}


template<class KEY, class VALUE>
int switch_mylist_head<KEY, VALUE>::erase(const KEY & k)
{
    list_head<KEY, VALUE> * pos, *head, *n;

    //CThreadLock lock(&m_mutex);

    head = &m_list_entry;

    for (pos = head->next, n = pos ->next; pos != head; pos = n, n = n->next)
    {
        if (pos->m_key == k)
        {
            delete []pos;
            m_node_num--;
        }
    }

    return 0;
}

template<class KEY, class VALUE>
int switch_mylist_head<KEY, VALUE>::get_node_num()
{
    //CThreadLock lock(&m_mutex);

    return m_node_num;
}

template<class KEY, class VALUE>
void switch_mylist_head<KEY, VALUE>::display()
{
    list_head<KEY, VALUE> * head, *pos;

    //CThreadLock lock(&m_mutex);
    head = &m_list_entry;
   for (pos = head->next; pos != head; pos = pos->next)
   {
        std::cout << "key: " << pos->m_key << " " << "value: " << pos->m_value << "\n";
   }
}



#endif // BILIN_SWITCH_MYLIST_HEAD_H

