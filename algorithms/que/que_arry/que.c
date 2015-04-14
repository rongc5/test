#include "communal.h"



typedef struct que{
    int head;
    int tail;
    int count;
    void *buf;
    size_t size;
    enum{
        EMPTY,
        FULL,
        NORMAL
    }stat;
int (*pop)(struct que *st, void *dst);
int (*push)(struct que *st, void *src);
}QUE;

int push(QUE *st, void *src)
{
    if (st->stat == FULL){
        printf("the stack is full!\n");
        return -1;
    }

    size_t size = st->size;
    char *pdst, *psrc;
    pdst = (char *)st->buf;
    psrc = (char *)src;

    pdst = pdst + st->tail*size;
    memcpy(pdst, psrc, size);
    st->tail++; 
    st->tail = st->tail%st->count;
    if (st->tail == st->head)
        st->stat = FULL;
    else
        st->stat = NORMAL;

   return 0;
}

int pop(QUE *st, void *dst)
{
    if (st->stat == EMPTY){
        printf("the stack is empty!\n");
        return -1;
    }

    char *pdst, *psrc;
    pdst = (char *)dst;
    psrc = (char *)st->buf + st->head*st->size;

    memcpy(pdst, psrc, st->size);
    st->head++;
    st->head = st->head%st->count;
    if (st->tail == st->head)
        st->stat = EMPTY;
    else
        st->stat = NORMAL;

    return 0;
}

int stack_init(QUE *st, int count, size_t size)
{
    st->head = st->tail = 0;
    st->count = (count == 0? 100:count);
    st->size = size;

    st->buf = (char *)malloc(sizeof(size)*st->count);
    if (st->buf == NULL){
        return -1;
    }
    st->push =push;
    st->pop = pop;

    st->stat = NORMAL;
    return 0;
}

#ifdef TEST

typedef struct {
    int id;
}NODE;

int main(void)
{
    int i, count = 50, ret; 
    
    QUE st;
    NODE pnode;
    struct list_head *pos;
    srand(time(NULL));

    stack_init(&st, count, sizeof(NODE));

    printf("push start!!!\n");
    for (i = 0; i < count; i++){

        pnode.id = rand()%count;   
        st.push(&st, &pnode);
        printf("%-5d  ", pnode.id);            
        if (!(i%10) && i)
            printf("\n");
    }
    printf("\npush end!!!\n");
    
    printf("pop start!!!\n");
    for (i = 0; i < count; i++){
        ret = st.pop(&st, &pnode);
            if (ret){
            break;
        }

        printf("%-5d  ", pnode.id);
        if (!(i%10) && i)
            printf("\n");
    }
    printf("\npop end!!!\n");

    return 0;
}
#endif
