#include <iostream>
#include <map>

using namespace std;

int main(int argc, char *argv[])
{
    map<int, int> mymap;
    map<int, int>::iterator it;

    mymap.insert(pair<int, int>(1, 1));
    mymap.insert(pair<int, int>(2, 2));
    mymap.insert(pair<int, int>(3, 3));

    //mymap.erase(5);

    for (it = mymap.begin(); it != mymap.end(); ++it)
        cout << it->first << " " << it->second << '\n';    

    for (auto &x: mymap)
        cout << x.first << " " << x.second << '\n';    


    return 0;
}
