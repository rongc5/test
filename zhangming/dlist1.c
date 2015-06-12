#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct stu {
    char name[32];
    int id;
    float avg;
    struct stu *next;
    struct stu *prev;
};

struct stu head;

#define list_for_each(pos, head) \
        for (pos = head->next; pos != head; pos = pos->next)
#define list_for_each_safe(pos, n, head) \
        for (pos = head->next, n = pos->next; pos != head; pos = n, n = n->next)

static void show_info(struct stu *node)
{
    printf("name = %s\n", node->name);
    printf("score = %f\n", node->avg);
}

void list_show(struct stu *head)
{
    struct stu *pos;

    list_for_each(pos, head) {
        show_info(pos);
    }
}

void list_destory(struct stu *head)
{
    struct stu *pos, *n;

    list_for_each_safe(pos, n, head) {
//    list_for_each(pos, head) {
        printf("free %p\n", pos);
        free(pos);
    }
}

struct stu * find_stu_by_name(struct stu *head, const char *name)
{
    struct stu *pos;

    list_for_each(pos, head) {
        if (strcmp(pos->name, name) == 0)
            return pos;
    }

    return NULL;
}

void init_dlist(struct stu *head)
{
    memset(head, 0, sizeof(*head));
    head->next = head;
    head->prev = head;
}

void __list_add(struct stu *prev, struct stu *next, struct stu *new)
{
    prev->next = new;
    new->prev = prev;
    new->next = next;
    next->prev = new;
}

void list_add_head(struct stu *head, struct stu *new)
{
    __list_add(head, head->next, new);
}

void list_add_tail(struct stu *head, struct stu *new)
{
    __list_add(head->prev, head, new);
}

void list_del(struct stu *head, struct stu *node)
{
    if (!node || head == node) {
        fprintf(stderr, "can't delete this node!\n");
        return;
    }
    struct stu *prev = node->prev;
    struct stu *next = node->next;

    prev->next = next;
    next->prev = prev;
    free(node);
}

int main(void)
{
    int i;

    struct stu *ptr[10];
    for (i = 0; i < 10; i++) {
        ptr[i] = (struct stu *)malloc(sizeof(struct stu));
        if (!ptr[i]) {
            perror("malloc");
            exit(1);
        }
    }

    init_dlist(&head);
    for (i = 0; i < 10; i++) {
        ptr[i]->id = i;
        ptr[i]->avg = rand()%51 + 50;
        sprintf(ptr[i]->name, "a%d", i);
        list_add_tail(&head, ptr[i]);
    }

    list_show(&head);

    list_destory(&head);

    return 0;
}


