#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <vector>

using namespace std;


int main(int argc, char *argv[])
{
    string url;
    //url.append("sz000963");
    url.append("Test string");

    //printf("%s\n", url.c_str());
    cout << url.capacity() <<"->" << url.size() << "->" << url.length() <<endl;

    printf("========\n");

    url.resize(url.size());
    cout << url.capacity() <<"->" << url.size() << "->" << url.length() <<endl;

    vector<string> vc;
    vc.push_back(url);
    cout << vc.capacity() <<"->" << vc.size() << "->" <<endl;

    return 0;
}
