#include <iostream>
#include <stdio.h>

using namespace std;


int main(int argc, char *argv[])
{
    int a = 2;
    decltype(a) b = 1.1;

    cout<< b << endl;
    printf("%d\n", b);   

    return 0;
}
