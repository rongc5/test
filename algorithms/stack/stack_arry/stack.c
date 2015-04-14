#include "communal.h"

typedef struct{
    int label;
    int count;
    void *buf;
    size_t size;
int (*push)(void *dst, int *label, int max, void *src, size_t size);
int (*pop)(void *dst, int *label, void *src, size_t size);
}STACK;

int push(void *dst, int *label, int max, void *src, size_t size)
{
    if (*label > max){
        printf("the stack is full!\n");
        return -1;
    }
    char *pdst, *psrc;
    pdst = (char *)dst;
    psrc = (char *)src;

    pdst = pdst + *label*size;
    memcpy(pdst, psrc, size);
    (*label)++;

   return 0;
}

int pop(void *dst, int *label, void *src, size_t size)
{
    int tmp;
    tmp = *label;
    (*label)--;

    char *pdst, *psrc;
    pdst = (char *)dst;
    psrc = (char *)src;

    if (*label < 0){
        *label = tmp;
        printf("the stack is empty!\n");
        return -1;
    }
    psrc = psrc + *label*size;
    memcpy(pdst, psrc, size);

    return 0;
}

int stack_init(STACK *st, int count, size_t size)
{
    st->label = 0;
    st->count = (count == 0? 100:count);
    st->pop = pop;
    st->push = push;
    st->size = size;

    st->buf = (char *)malloc(sizeof(size)*st->count);
    if (st->buf == NULL){
        return -1;
    }
    
    return 0;
}

#ifdef TEST

typedef struct {
    int id;
}NODE;

int main(void)
{
    int i, count = 100, ret; 
    
    STACK st;
    NODE pnode;
    struct list_head *pos;
    srand(time(NULL));

    stack_init(&st, count, sizeof(NODE));

    printf("push start!!!\n");
    for (i = 0; i < count; i++){

        pnode.id = rand()%count;   
        st.push(st.buf, &st.label, st.count, &pnode, sizeof(NODE));
        printf("%d  ", pnode.id);            
        if (!(i%10) && i)
            printf("\n");
    }
    printf("\npush end!!!\n");
    
    printf("pop start!!!\n");
    for (i = 0; i < count; i++){
        memset(&pnode, 0, sizeof(NODE));
        ret = st.pop(&pnode, &st.label, st.buf, sizeof(NODE));
            if (ret){
            break;
        }

        printf("%d  ", pnode.id);
        if (!(i%10) && i)
            printf("\n");
    }
    printf("\npop end!!!\n");

    return 0;
}
#endif
