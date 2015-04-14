#include "bilin_switch_mylist_head.h"
#include <string>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    string tmp;

    switch_mylist_head<int, string> mlist;
    mlist.push_back_uniq(1, "hello");
    
    mlist.push_back_uniq(1, "qianfeng");
    mlist.push_back_uniq(2, "zm");
    mlist.push_back_uniq(3, "qqqqqq");

    mlist.push_back(3, "qqqqqq");

    std::cout << mlist.get_node_num() << '\n';
    
    mlist.find_value(2, &tmp);
    std::cout << tmp.c_str() << '\n';

    mlist.display();

    //mlist.erase_uniq(3);
    //mlist.erase(3);
    //mlist.display();

    std::cout<<"\n\n";
    mlist.clear();
    mlist.display();


    return 0;
}

