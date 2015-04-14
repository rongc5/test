#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef char elmType;
#define ELMSIZE 65536

typedef enum
{
    EMPTY,
    NORMAL,
    FULL
}Que_status_t;

typedef struct que
{
    elmType buf[ELMSIZE];
    int head;
    int tail;
    Que_status_t status;
}Que_t;

Que_t * create_and_init_que(void)
{
    Que_t * qp = (Que_t *)malloc(sizeof(Que_t));
    if (!qp)
    {
        perror("malloc");
        return NULL;
    }

    qp->head = qp->tail = 0;
    qp->status = EMPTY;

    return qp;
}

void mycpy(char *dst, char *src, size_t size)
{
    while (size--)
    {
        *(dst++) = *(src++);
    }
}

int read_one_elm(Que_t *qp, elmType *e)
{
    if (qp->status == EMPTY)
        return -1;
    mycpy((char *)e, (char *)(qp->buf+qp->head), sizeof(elmType));
    qp->tail++;
    if (qp->tail == ELMSIZE)
        qp->head = 0;
    if (qp->head == qp->tail)
        qp->status = EMPTY;
    else
        qp->status = NORMAL;

    return 0;
}

int read_que(Que_t *qp, const void *buf, size_t size)
{
    int i, ret;
    elmType *base = (elmType *)buf;

    for (i = 0; i < size; i++)
    {
        ret = read_one_elm(qp, base + i);
        if (-1 == ret)
            break;
    }

    return i;
}

int write_one_elm(Que_t *qp, elmType *e)
{
    if (qp->status == FULL)
        return -1;
    mycpy((char *)(qp->buf + qp->tail), (char *)e, sizeof(elmType));
    qp->head++;
    if (qp->head == ELMSIZE)
        qp->tail = 0;
    if (qp->head == qp->tail)
        qp->status == FULL;
    else 
        qp->status = NORMAL;

    return 0;
}

int write_que(Que_t *qp, const void *buf, size_t size)
{
    int i, ret;
    elmType *base = (elmType *)buf;

    for (i = 0; i < size; i++)
    {
        ret = write_one_elm(qp, base + i);
        if (-1 == ret)
            break;
    }

    return i;
}

int main(void)
{
    Que_t *qp;
    int ret;
    char buf[12];

    qp = create_and_init_que();
    if (!qp)
        exit(1);

    ret = write_que(qp, "qianfeng", 12);
    if (ret < 12)
    {
        printf("que was full!\n");
    }

    ret = read_que(qp, buf, 12);
    buf[ret] = '\0';

    printf("%s(%d)\n", buf, ret);
    ret = read_que(qp, buf, 12);
    printf("ret = %d\n", ret);

    free(qp);
    exit(0);
}


