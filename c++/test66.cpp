#include "myhash.h"
#include <string>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    switch_myhash_table<int, string> mlist;
    mlist.add_key_value(1, "hello");
    
    mlist.add_key_value(1, "qianfeng");
    mlist.add_key_value(2, "zm");
    mlist.add_key_value(3, "qqqqqq");

    //mlist.push_back(3, "qqqqqq");

    //std::cout << mlist.get_node_num() << '\n';
    string tmp;
    mlist.get_value(1, tmp);
    std::cout<< tmp.c_str() << '\n';

    mlist.display();

    //mlist.erase_uniq(3);
    //mlist.erase(3);
    //mlist.display();

    std::cout<<"\n\n";
    mlist.clear();
    mlist.display();


    return 0;
}

