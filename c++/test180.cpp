#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

int main(int argc, char *argv[])
{
    string str = "hello world";
    unordered_map<char, int> nm;

    for (const auto ch: str)
    {
        nm[ch]++;
    }

    for (auto ii:nm)
    {
        cout << ii.first << ":" <<ii.second <<'\n';
        nm[ii.first] += 2;
    }

    for (auto ii:nm)
    {
        cout << ii.first << ":" <<ii.second <<'\n';
    }

    return 0;
}
