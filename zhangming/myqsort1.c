#include <stdio.h>

#define SIZE 20

void mycpy(char *a, char *b, size_t size)
{
    while (size--)
        *(a++) = *(b++);
}

void swap(void *a, void *b, size_t size)
{
    char tmp[size];

    mycpy(tmp, (char *)a, size);
    mycpy((char *)a, (char *)b, size);
    mycpy((char *)b, tmp, size);
}

void myqsort(void *array, size_t nmemb, size_t size, int (*compare)
        (const void *a, const void *b))
{
    if (nmemb <= 1)
        return;
    int index, i;
    char *base;
    char *begin;

    begin = (char *)array;
    base = (char *)array + (nmemb - 1)*size;
    index = 0;

    for (i = 0; i < nmemb - 1; i++)
    {
        if (compare(begin + i*size, base) < 0)
        {
            if (i != index)
                swap(begin + i*size, begin + index*size, size);
            index++;
        }
    }

    if (index < nmemb - 1)
        swap(begin + index*size, base, size);
    myqsort(begin, index, size, compare);
    myqsort(begin + (index + 1)*size, nmemb - index - 1, size, compare);
}

void display(int *a, int size)
{
    int i;

    for (i = 0; i < size; i++)
        printf("%d: ", a[i]);
    printf("\b \n");
}

int array_compare(const void *a, const void *b)
{
    int *m = (int *)a;
    int *n = (int *)b;

    if (*m > *n)
        return -1;
    else if (*m < *n)
        return 1;
    else
        return 0;
}

int main(void)
{
    int a[SIZE];
    int i;
    srand(getpid());

    for (i = 0; i < SIZE; i++)
    {
        a[i] = rand()%100;
    }

    display(a, SIZE);
    myqsort(a, SIZE, sizeof(int), array_compare);
    display(a, SIZE);

    return 0;
}
