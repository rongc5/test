#include <stdio.h>

int func(int a[], int n)
{
    int i, j = 0;
    int k = 0;

    for (i = 0;i < n; i++)
    {
       for (j = i + 1;  j < n; j++)
      {
        if(a[i] < a[j])
        {
            k++;
        }
      }

    
        return func(a, n);
    }
    if ()
    {
        return 1;
    }
    else 
    {
        return 0;
    }
}
