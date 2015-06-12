#include <stdio.h>

#if 0
void func(int s);
int a;
char b;
int *p = &a;
int *p = &b;
int *a;
int * *p = &a;
int **a;
int ** *p = &a;

int a[10];

a typeof(a) != int;
sizeof(a[0]) = 4;
sizeof(a) = 40;
int *p = a;

int *a[10];
int **p = a;

char *argv[10];
char **p = argv;

int a[3][5];
sizeof(a[0]) == 20;
int [5] *p = a;
int (*p)[5] = a;

void func(int s);
int a;
int *p = &a;

*p = func;
void (int s) *p = func;
void (*p)(int s);

signal(int s, void (*func)(int s));

int *p;
int *func(void);
void (*p)(int);

void (*signal(int s, void(*func)(int s)))(int);


#endif

int main(int c, char **v)
{
    printf("size = %d\n", sizeof(int [5]));

    return 0;
}
