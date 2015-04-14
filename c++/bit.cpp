#include <stdio.h>
#include <iostream>

using namespace std;

struct bit
{ int a:3;
    int b:2;
    int c:3;
};

int main()
{
    struct bit s;
    char *c=(char*)&s;
    cout<<sizeof(bit)<<endl;
    *c=0x99;
    cout << s.a <<endl <<s.b<<endl<<s.c<<endl;
    int a=-1;
    printf("%x",a);
    return 0;
}

