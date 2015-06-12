#ifndef __LIST_H__
#define __LIST_H__

#ifdef __cplusplus
extern "C"{
#endif

#include "communal.h"

#define LIST_FOR_EACH(pos, head) \
   for (pos = (head)->next; pos != (head); pos = pos->next)

#define LIST_ENTRY(ptr, type, member) \
    ((type *)((char *)(ptr) - (unsigned long long)(&((type *)0)->member)))

#define LIST_FOR_EACH_SAFE(pos, n, head) \
   for (pos = (head)->next, n = pos ->next; pos != (head); pos = n, n = n->next)

#define LIST_FREE(ptr, type, member) \
do { \
    struct list_head *pos, *n; \
    type *tmp; \
		LIST_FOR_EACH_SAFE(pos, n, ptr){ \
        tmp = LIST_ENTRY(pos, type, member);\
            free(tmp);\
    } \
}while (0)


struct list_head{
    struct list_head *next;
    struct list_head *prev;
};

void list_init(struct list_head * head);

void __list_add(struct list_head * prev, struct list_head * node, struct list_head * next);

void __list_link(struct list_head *prev, struct list_head * next);

void list_add_tail(struct list_head * head, struct list_head * node);

void list_add_head(struct list_head * head, struct list_head * node);

#ifdef __cpluscplus
}
#endif


#endif
