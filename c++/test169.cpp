#include <iostream>
#include <string>
#include <stdio.h>
#include <string.h>
#include <map>

using namespace std;

int main(int argc, char *argv[])
{
    map<int, int> mymap;
    mymap[3] = 10;
    mymap[5] = 15;

    map<int, int>::iterator it;

    it = mymap.upper_bound(6);
    if (it == mymap.end())
    {
        printf("not find\n");
    }

    return 0;
}
