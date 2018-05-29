#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>

using namespace std;


int main(int argc, char *argv[])
{
    string url = "/queryid?id=sz002285&history_days=5";

    printf("%s\n", url.c_str());

    printf("========\n");

    size_t pos = url.find("?");
    if (pos == std::string::npos)
    {   
        return 1;
    }
    string str(url.substr(0, pos));
    printf("%s\n", str.c_str());

    return 0;
}
