#include "communal.h"

int swap(int *a, int *b)
{
    int tmp;

    tmp = *a;
    *a = *b;
    *b = tmp;

    return 0;
}

int myisort(int a[], int num)
{
    int i, j;

    for (i = 1; i < num; i++)
        for (j = i - 1; j >= 0 && a[j] > a[j+1]; j--)
            swap(&a[j], &a[j+1]);

   return 0;
}

#ifdef TEST

int main(void)
{
    int count = 50, i;
    /*int *buf;*/

    /*buf = (int *)malloc(sizeof(int)*count);*/
    /*if (!buf){*/
        /*perror("malloc");*/
        /*return -1;*/
    /*}*/

    /*srand(time(NULL));*/

    int buf[6] = {1,2,3,4,6,5};

    /*for (i = 0; i < count; i++){*/
        /*buf[i] = rand()%count;    */
        /*printf("%-3d", buf[i]);*/
        /*if (i && !(i%10))*/
            /*printf("\n");*/
    /*}*/
    /*printf("\n\n");*/

    myisort(buf, 6);


    for (i = 0; i < 6; i++){
        printf("%-3d", buf[i]);
        if (i && !(i%10))
            printf("\n");
    }
    printf("\n");
    
    return 0;
}


#endif
