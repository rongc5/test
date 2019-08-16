#include <iostream>
#include  <utility>
#include <string>


using namespace std;

int main(int argc, char *argv[])
{
    string str1 = "hello";
    string str2 = "zm";

    str1 = move(str2);

    cout << "str1:" << str1 << "\n";

    return 0;
}
