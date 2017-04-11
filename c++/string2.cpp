#include <iostream>
#include <string>
#include <sstream>
#include "stdio.h"
#include "stdlib.h"
#include <pthread.h>
#include <string.h>

using namespace std;

class A
{
    public:
        operator std::string () const
        {
            stringstream stream;
            stream << "hello\n";

            //cout << stream.str();
            return stream.str();
        }
};


int main(int argc, char *argv[])
{
    A a;
    //cout << str.c_str();
    //a.string();
    //cout << a;

    string str;

    sprintf((char *)str.c_str(), "%s %s", "hello world", "123456");

    printf("%s\n", str.c_str());


    return 0;
}
