#include <iostream>
#include <map>
#include <string>

using namespace std;

int main()
{
    typedef std::multimap<int, string> IntStringMMap;

    IntStringMMap coll;
    coll.insert(make_pair(1, "strings"));
    coll.insert(make_pair(6, "is"));
    coll.insert(make_pair(3, "multimap"));

    IntStringMMap::const_iterator pos;

    for (pos = coll.begin(); pos != coll.end(); ++pos){
        std::cout << pos->second << ' ';
    }

    cout << endl;

    return 0;
}
