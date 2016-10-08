#include <stdio.h>
#include <iostream>

using namespace std;

struct AA{
    int x;
    static int y;
};

int AA::y;


int main(int argc, char *argv[])
{
    AA a;
    printf("%d %d\n", sizeof(AA), sizeof(a));

    return 0;
}
