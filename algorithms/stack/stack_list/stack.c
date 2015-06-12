#include "list.h"
#include "communal.h"

typedef struct{
    struct list_head list_entry;
    int (*push)(struct list_head *head, struct list_head *node);
    int (*pop)(struct list_head *head, struct list_head **node);
}STACK;

int push(struct list_head *head, struct list_head *node)
{
   list_add_head(head, node);

   return 0;
}

int pop(struct list_head *head, struct list_head **node)
{
    struct list_head *pos = head->next; 

    if (pos == head){
        printf("stack is empty!\n");
        *node == NULL;
        return -1;
    }
    
    *node = pos;
    __list_link(head, pos->next);

    return 0;
}

int stack_init(STACK *st)
{
    list_init(&st->list_entry);
    st->pop = pop;
    st->push = push;

    return 0;
}

#ifdef TEST

typedef struct {
    int id;
    struct list_head list_entry;
}NODE;

int main(void)
{
    int i, count = 100, ret; 
    
    STACK st;
    NODE *pnode;
    struct list_head *pos;
    srand(time(NULL));

    stack_init(&st);

    printf("push start!!!\n");
    for (i = 0; i < count; i++){
        pnode = (NODE *)malloc(sizeof(NODE));
        if (!pnode){
            perror("malloc");
            i--;
            continue;
        }

        pnode->id = rand()%count;   
        st.push(&st.list_entry, &pnode->list_entry);
        printf("%-5d  ", pnode->id);            
        if (!(i%10) && i)
            printf("\n");
    }
    printf("\npush end!!!\n");
    
    printf("pop start!!!\n");
    for (i = 0; i < count; i++){
        ret = pop(&st.list_entry, &pos);
        if (ret){
            break;
        }

        pnode = LIST_ENTRY(pos, NODE, list_entry); 
        printf("%-5d  ", pnode->id);
        free(pnode);
        if (!(i%10) && i)
            printf("\n");
    }
    printf("\npop end!!!\n");

    return 0;
}
#endif
