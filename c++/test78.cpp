#include <iostream>
#include <map>

using namespace std;

int main(int argc, char *argv[])
{
    map<int, int> mymap;
    map<int, int>::iterator it;

    mymap.insert(pair<int, int>(1, 1));
    mymap.insert(pair<int, int>(1, 2));
    mymap.insert(pair<int, int>(1, 3));


    for (it = mymap.begin(); it != mymap.end(); ++it)
        cout << it->first << " " << it->second << '\n';    

    return 0;
}
