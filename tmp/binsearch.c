#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define binSearch(a, sum, member, obj, id) \
    do {\
        id = -1;\
        int high, low, mid; \
        low = 0, high = sum - 1; \
        if (obj > a[high].member || obj < a[low].member){ \
            break; \
        } else {\
            while (low <= high){ \
                mid = (high + low)/2; \
                    if (obj < a[mid].member) \
                        high = mid - 1; \
                    else if (obj > a[mid].member) \
                        low = mid + 1; \
                    else if (a[mid].member== obj){ \
                        id = mid; \
                            break;\
                    }\
            }\
        }\
    }while (0)


typedef struct{
    int i;
}DATA;


DATA a[]={1, 3, 9, 15, 18, 33, 39, 25};

int main(void)
{
    int id;
    binSearch(a, sizeof(a)/sizeof(int), i, 10, id);

    if (id == -1){
        printf("%d\n", id);
        return ;
    }

    printf("id:%d, %d\n", id, a[id]);

    return 0;
}
