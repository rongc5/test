#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <vector>

using namespace std;


int main(int argc, char *argv[])
{
    string str = "3M12G9M";
    cout<< str.substr(5, str.size()-5) << endl;

    return 0;
}
