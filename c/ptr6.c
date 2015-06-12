#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
   int *ptr[10];

   int *a = (int *)ptr;

   printf("%p\n", a);
   ++a;
   printf("%p\n", a);

   return 0;
}   
