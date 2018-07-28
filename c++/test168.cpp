#include <iostream>
#include <string>
#include <set>

using namespace std;

int main(int argc, char *argv[])
{
    
    set<string> t;
    t.insert("20180101");
    t.insert("20181003");
    t.insert("20180621");
    t.insert("20170404");
    t.insert("20170528");
    t.insert("20091005");
    t.insert("20180618");

    for (set<string>::iterator ii = t.begin(); ii != t.end(); ii++)
        cout << *ii << endl;

    return 0;
}

