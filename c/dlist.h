#ifndef __DLIST_H_
#define __DLIST_H_




struct list_head {
	struct list_head *next;
	struct list_head *prev;
};

void list_add_head(struct list_head *head, struct list_head *new);

void list_add_tail(struct list_head *head, struct list_head *new);

void list_del(struct list_head *node);





#endif
