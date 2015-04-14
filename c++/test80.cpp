#include <iostream>
#include <list>
#include <algorithm>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
    list<int> mylist;

    for (int i = 0; i < 10; i++)
        mylist.push_back(i);

    list<int>::iterator it;

    it = find(mylist.begin(), mylist.end(), 10);
    if (it == mylist.end())
        mylist.push_back(10);
        //it->push_back(10);

    for (it = mylist.begin(); it != mylist.end(); ++it)
        cout << *it <<" ";

    cout <<'\n';

    return 0;
}


