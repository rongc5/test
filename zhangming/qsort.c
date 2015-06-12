#include <stdio.h>

#define SIZE 10

void swap(int *a, int *b)
{
    int t;

    t = *a;
    *a = *b;
    *b = t;
}

/*void qsort(int *a, int low, int high)
{
    if (low >= high)
        return;
    int base, index, i;

    base = a[high];
    index = low;

    for (i = low; i <= high; i++) {
        if (a[i] < base) {
            if (i != index)
                swap(a+i, a+index);
            index++;
        }
    }
    
    swap(a+index, a+high);
    qsort(a, low, index-1);
    qsort(a, index+1, high);
}    */

void display(int *a, int size)
{
    int i;

    for (i = 0; i < size; i++) {
        printf("%d:", a[i]);
    }
    printf("\b \n");
}


void myqsort(int a[], int low, int high)
{
    
    int dp;
    if (low < high)
    {    
        dp = par(a, low, high);
        myqsort(a, low, dp - 1);
        myqsort(a, dp + 1, high);
    }
}

int par(int a[], int low, int high)
{
    int la, lb, lc, tmp;
    
    lc = a[low];
    la = low - 1;
    lb = high + 1;
    
    while ((la + 1) != lb)
    {
        if (a[la + 1] <= lc)
        la++;
        else if(a[lb - 1] > lc)
        lb--;
        else
        {
            tmp = a[la + 1];
            a[++la] = a[lb - 1];
            a[--lb] = tmp;
        }
        
    }
    
    a[low] = a[la];
    a[la] = lc;
    
    return la;
}

int compar(const void *p1, const void *p2)
{
    return strcmp(*(char *const *)p1, *(char *const *)p2);
}
int main(void)
{
    int a[SIZE];
    int i;
    srand(getpid());
    
    for (i = 0; i < SIZE; i++) {
        a[i] = rand() % 100;
    }

    display(a, SIZE);
    myqsort(a, 0, SIZE-1);
    display(a, SIZE);


}
