#include <iostream>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
    char * ptr = NULL;
    //err
   // string str(ptr);
   string str("hello world");

    cout << str.c_str();

    return 0;
}
