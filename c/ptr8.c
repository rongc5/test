#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    /*int a = 6;*/
    /*int *ptr = &a;*/
    /*int **ptr = &ptr;*/
    /*int *ptr[10];*/
    /*int *[10] *p = &ptr*/

    /*char *p;*/
    /*void *p;*/
    /*p++;*/
    
    /*char *ptr;*/
    void *ptr;

   printf("%d\n", *(int*)ptr);
   ptr++;
   printf("%d\n", *(int *)ptr);

   return 0;
}   
