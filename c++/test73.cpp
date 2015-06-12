#include <iostream>
#include <string.h>
#include <string>
#include <list>
#include <map>

using namespace std;

int main(int argc, char *argv[])
{

    map< int, list<string> > mymap;
    map< int, list<string> >::iterator it;



    it = mymap.find(1);
    if (it == mymap.end())
    {
        list<string> dlist;
        dlist.push_back("hello");

        mymap.insert(pair< int, list<string> >(1, dlist));
    }

    it = mymap.find(1);
    if (it != mymap.end())
    {

        it->second.push_back("world");
        it->second.push_back("qianfeng");
    }
    
    list<string>::iterator itm;
    for (it = mymap.begin(); it != mymap.end(); ++it){
            cout << it->first << " ";
            for (itm = it->second.begin(); itm != it->second.end(); ++itm)
                cout << itm->c_str() << " ";
            cout << '\n';
    }
            //cout<<"hello" << endl;
    
    
        

    return 0;
}
