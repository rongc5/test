#include <iostream>
#include <set>

using namespace std;

int main()
{
    typedef std::set<int> IntSet;

    IntSet coll;
    coll.insert(1);
    coll.insert(6);
    coll.insert(3);

    IntSet::const_iterator pos;

    for (pos = coll.begin(); pos != coll.end(); ++pos){
        std::cout << *pos << ' ';
    }

    cout << endl;

    return 0;
}
