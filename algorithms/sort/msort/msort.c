#include "communal.h"

int merger(int a[], int first, int mid, int last, int tmp[])
{
    int i, j, m, n, k;

    m = mid, n = last, i = first, j = mid + 1, k = 0;

    while (i <= m && j <= n)
        if (a[i] < a[j]){
            tmp[k++] = a[i++];
        }else {
            tmp[k++] = a[j++];
        }
        //tmp[k++] = (a[i] < a[j]?a[i++]:a[j]);

    //printf("m:%d, n:%d, i:%d, j:%d, k:%d\n", m, n, i, j, k);
    while (i <= m)
        tmp[k++] = a[i++];

    while (j <= n)
        tmp[k++] = a[j++];

    for (i = 0; i < k; i++)
        a[first + i] = tmp[i];

    return 0;
}

int mymsort(int a[], int first, int last, int tmp[])
{
    int mid;

    if (first < last){
        mid = (first + last)/2;
        mymsort(a, first, mid, tmp);
        mymsort(a, mid + 1, last, tmp);
        merger(a, first, mid, last, tmp);
    }

   return 0;
}

#ifdef TEST

int main(void)
{
    int count = 50, i;
    int *tmp;

    int buf[6] = {1, 2, 3, 4, 6, 5};
    /*buf = (int *)malloc(sizeof(int)*count);*/
    /*if (!buf){*/
        /*perror("malloc");*/
        /*return -1;*/
    /*}*/

    tmp = (int *)malloc(sizeof(int)*count);
    if (!tmp){
        perror("malloc");
        return -1;
    }

    /*srand(time(NULL));*/

    /*for (i = 0; i < count; i++){*/
        /*buf[i] = rand()%count;    */
        /*printf("%-3d", buf[i]);*/
        /*if (i && !(i%10))*/
            /*printf("\n");*/
    /*}*/
    /*printf("\n\n");*/

    mymsort(buf, 0, 5, tmp);
    free(tmp);

    for (i = 0; i < 6; i++){
        printf("%-3d", buf[i]);
        if (i && !(i%10))
            printf("\n");
    }
    printf("\n");
    
    return 0;
}


#endif
