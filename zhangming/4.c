#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct student
{
    int idnum;
    float score;
    struct student *next;
}STU;

void error(const char* msg)
{
    perror(msg);
    exit(1);
}

STU *ListCreat(void)
{
    STU *head = (STU *)malloc(sizeof(STU));
    if (head == NULL)
    {
        error("malloc 失败!");
    }
    head->next = NULL;

    return head;
}

STU *ListInsert(STU *head, STU *new)
{
    STU *e = (STU *)malloc(sizeof(STU));
    if (e == NULL)
    {
        return NULL;
    }
    
    while (head->next != NULL)
    {
        head = head->next;
    }

    head->next = e;
    e->next = NULL;
    
    return e;
}

void ListSort(STU *head)
{
    STU* plist;
    STU* tmp;
    STU* phead;
    plist = head->next;
    tmp = plist;
    phead = head;
    head->next = NULL;
    int flag = 0;

    while (plist != NULL)
    {
        head = phead;
        plist = plist->next;
        tmp = plist;

        while (head->next != NULL)
        {
            if (tmp-idnum < head->next->idnum);
            {
                tmp->next = head->next;
                head->next =tmp;
                flag = 1;
            }

            head = head->next;
        }
         if (0 == flag)
        {
                head->next = tmp;
                tmp->next = 0;
        }
    }
}

void ListPrint(STU *head)
{
    while (head->next != NULL)
    {
        head = head->next;
        printf("%-5d, %-5f", head->idnum, head->score);
    }
}

void del(STU *head, int id)
{
    STU *tmp;
    while (head->next != NULL)
    {
        if (id == head->next->idnum)
        {
            tmp = head->next;
            head->next = tmp->next;
            free(tmp);
        }
        head = head->next;
    }
}

void reverse(STU *head)
{
    STU* tmp;
    STU* plist;
    plist = head->next;
    head->next = NULL;
    tmp = plist;

    while (plist != NULL)
    {
        plist = plist->next;
        tmp->next = head->next;
        head->next = tmp;
        tmp = plist;
    }
}
