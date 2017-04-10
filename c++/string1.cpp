#include <iostream>
#include <string>
#include <sstream>
#include "stdio.h"
#include "stdlib.h"
#include <pthread.h>

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

#define  LOG_COMMON(fmt, ...) \
    do { \
    stringstream stream; \
    stream << pthread_self() << __FILE__ << ":" << __LINE__ << ":";\
    cout << stream.str(); \
    } while (0)


int main(int argc, char *argv[])
{
    A a;
    //cout << str.c_str();
    //a.string();
    //cout << a;
    string b = a;
    cout << b.c_str();


    LOG_COMMON("123456789");

    return 0;
}
