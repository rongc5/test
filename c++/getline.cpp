#include <iostream>
#include <string>
#include <sstream>

using namespace std;

int main(int argc, char *argv[])
{
    string str;

    while (getline(cin, str))
    {
    	stringstream ss;
        ss << str;
        cout << str << "==>";
        while (ss >> str)
        {
            cout << str << ":";
        }
        cout << endl;
    }

    return 0;
}
