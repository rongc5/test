#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct data{
    int id;
    struct data *next;
}DATA;

void list_init(DATA *data)
{
    data->id = 0;
    data->next = NULL;
}

void list_insert(DATA *head, DATA *node)
{
    DATA *e = (DATA *)malloc(sizeof(DATA));
    if (!e){
        perror("malloc");
        return ;
    }

    e->id = node->id;

    while (head->next)
        head = head->next;

    head->next = e;
    e->next = NULL;
}

void list_reverse(DATA *head)
{
    DATA *list1, *list2, *list3;
    list1 = head->next;
    list2 = list1->next;

    while (list2)
    {
        list3 = list2->next;
        list2->next = list1;
        list1 = list2;
        list2 = list3;
    }

    head->next->next = NULL;  

    head->next = list1;
}

void list_sort(DATA *head)
{
    DATA *phead = head;
    DATA *list = head->next;
    DATA *tmp;
    phead->next = NULL;

    while (list){
        tmp = list->next;
        while (phead->next){
            if (phead->next->id > list->id)
                break;
            phead = phead->next;
        }

        list->next = phead->next;
        phead->next = list;
     //   printf("%-5d    \n", list->id);
        list = tmp;
        phead = head;
    }

    //printf("\n");
}

void list_display(DATA *head)
{
    DATA *plist = head->next;

    while (plist)
    {
        printf("%-3d    ", plist->id);
        plist = plist->next;
    }
    printf("\n");
}

void list_free(DATA *head)
{
    DATA *e;

    head = head->next;
    while (head){
        e = head;
        head = head->next;
        free(e);
    }
}

#ifdef TEST

DATA g_head;

int main(int c, char **v)
{
    DATA node;
    int i;

    list_init(&g_head);

    srand(1);

    for (i = 0; i < 10; i++){
        node.id = rand()%100;
        list_insert(&g_head, &node);
    }

    list_display(&g_head);

    list_reverse(&g_head);

    list_display(&g_head);

    list_sort(&g_head);

    list_display(&g_head);

    list_free(&g_head);

    return 0;
}

#endif

