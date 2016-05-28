#include "communal.h"

int swap(int a[], int low, int high)
{
    int x;
    x = a[low];

    while (low < high){
        while (low < high && a[high] > x)
            high--;

        if (low < high){
            a[low] = a[high];
            low++;
        }

        while (low < high && a[low] < x)
            low++;

        if (low < high){
            a[high] = a[low];
            high--;
        }
    }
    a[low] = x;

    return low;
}

int myqsort(int a[], int low, int high)
{
    int k, i;

   if (low < high){
       k = swap(a, low, high);
       myqsort(a, low, k - 1);
       myqsort(a, k + 1, high);
   }

   return 0;
}
#ifdef TEST

int main(void)
{
    int count = 6, i;
    /*int *buf;*/

    /*buf = (int *)malloc(sizeof(int)*count);*/
    /*if (!buf){*/
        /*perror("malloc");*/
        /*return -1;*/
    /*}*/

    /*srand(time(NULL));*/

    /*for (i = 0; i < count; i++){*/
        /*[>buf[i] = rand()%count;    <]*/
        /*buf[i] = rand()%count;    */
        /*printf("%-3d", buf[i]);*/
        /*if (i && !(i%10))*/
            /*printf("\n");*/
    /*}*/
    /*printf("\n");*/

    int buf[6] = {1,2,3,4,6,5};
    myqsort(buf, 0, 5);


    for (i = 0; i < 6; i++){
        printf("%-3d", buf[i]);
        if (i && !(i%10))
            printf("\n");
    }
    printf("\n");
    
    return 0;
}


#endif
