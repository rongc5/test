#include <iostream>
#include <string>
#include <map>
#include <stdio.h>

using namespace std;

int main(int argc, char *argv[])
{
    map<int, map<int, int> > mm;
    map<int, map<int, int> >::iterator it;

    mm[0][0] = 1;
    mm[0][1] = 2;
    mm[0][2] = 3;

    it = mm[0][1].find(1);
    if (it != mm[0].end())
        printf("%d\n", it->second);
    
    printf("%d\n", mm[0][1]);
    //for (it = mm.begin(); it != mm.end(); ++it)
    //{
        //printf("%d %d %d\n", it->first, it->second.first, )
    //}

    std::cout << mm.size() << '\n';

    return 0;
}
