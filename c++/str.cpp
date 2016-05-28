#include <string>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    string str = "hello world";

    string::iterator it = str.begin();
    cout << *it << '\n';

    it = str.end();

    cout << *it << '\n';
    
    return 0;
}
