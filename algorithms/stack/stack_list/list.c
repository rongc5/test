#include "list.h"
#include "communal.h"

void list_init(struct list_head * head)
{
    head->next = head;
    head->prev = head;
}

void __list_add(struct list_head * prev, struct list_head * node, struct list_head * next)
{
    prev->next = node;
    node->prev = prev;
    next->prev = node;
    node->next = next;
}

void __list_link(struct list_head *prev, struct list_head * next)
{
    prev->next = next;
    next->prev = prev;
}


void list_add_tail(struct list_head * head, struct list_head * node)
{
    struct list_head * prev, *next;

    prev = head->prev;
    next = head;
    __list_add(prev, node, next);
}

void list_add_head(struct list_head * head, struct list_head * node)
{
    struct list_head * prev, * next;
    prev = head;
    next = head->next;
    __list_add(prev, node, next);
}


