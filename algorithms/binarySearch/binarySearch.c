#include <stdio.h>

int search(int a[], int num, int tmp)
{
    int high, low, mid;
    low = 0, high = num - 1;
    if (tmp > a[high] || tmp < a[low]){
        printf("I can't find the num %d\n", tmp);
        return -1;
    }

    while (low <= high){
        mid = (high + low)/2;
        if (a[mid] > tmp) 
            high = mid - 1;
        else if (a[mid] < tmp)
            low = mid + 1;
        else if (a[mid] == tmp){
            printf("I find it a[%d]=%d\n", mid, a[mid]);
            return mid;
        }
    }
    printf("I don't find it");
    return -1;
}


int main(void)
{
    int a[] = {1, 3, 5, 6, 9, 10, 15, 33, 22, 99, 101, 202, 999};
    int tmp;

    while (1){
        printf("请输入你要查找的数据!:  ");
        fflush(stdout);
        scanf("%d", &tmp);
        search(a, sizeof(a)/sizeof(a[0]), tmp);
    }
    return 0;
}
