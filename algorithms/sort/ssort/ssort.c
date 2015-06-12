#include "communal.h"

int swap(int *a, int *b)
{
    int tmp;

    tmp = *a;
    *a = *b;
    *b = tmp;

    return 0;
}

int myssort(int a[], int num)
{
    int i, j;

    for (i = 0; i < num; i++)
        for (j = i; j < num; j++)
            if (a[i] > a[j])
                swap(&a[i], &a[j]);

   return 0;
}
#ifdef TEST

int main(void)
{
    int count = 50, i;
    int *buf;

    buf = (int *)malloc(sizeof(int)*count);
    if (!buf){
        perror("malloc");
        return -1;
    }

    srand(time(NULL));

    for (i = 0; i < count; i++){
        buf[i] = rand()%count;    
        printf("%-3d", buf[i]);
        if (i && !(i%10))
            printf("\n");
    }
    printf("\n\n");

    myssort(buf, count);


    for (i = 0; i < count; i++){
        printf("%-3d", buf[i]);
        if (i && !(i%10))
            printf("\n");
    }
    printf("\n");
    
    return 0;
}


#endif
