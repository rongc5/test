#include "dlist.h"

#define OFFSETOF(member, type) ((unsigned long )&(((type *)0)->member))


#define container_of(ptr, type, member) (\
	(type *)((char *)ptr - OFFSETOF(member, type)))

#define list_entry(ptr, type, member) \
	container_of(ptr, type, member)

#define LIST_FOR_EACH(pos,head)\
    (pos = pos->next; pos != (head); pos = pos->next)

void __list_add(struct list_head *next, struct list_head *prev, 
	struct list_head *new) 
{
	prev->next = new;
	new->prev = prev;
	new->next = next;
	next->prev = new;
}


void list_add_head(struct list_head *head, struct list_head *new) 
{
	__list_add(head->next, head, new);
}

void list_add_tail(struct list_head *head, struct list_head *new) 
{
	__list_add(head, head->prev, new);
}

void list_del(struct list_head *node) 
{
	node->prev->next = node->next;
	node->next->prev = node->prev;
}
