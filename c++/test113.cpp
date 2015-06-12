#include <string>
#include <iostream>
#include <sstream>

using namespace std;

int main(int argc, char *argv[])
{
    string cmd = "hello ";

    stringstream ss;
    ss << 11;
    ss >> cmd;

    cout << cmd << '\n';

    return 0;
}
