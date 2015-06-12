#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
   int a[10]={0};

   int  (*ptr)[10] = &a;

   printf("%p, %u\n", ptr, ptr);
   ++ptr;
   printf("%p, %u\n", ptr, ptr);

   return 0;
}   
