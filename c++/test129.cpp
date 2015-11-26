#include <iostream>
#include <stdio.h>

using namespace std;


const char * test(const char * str)
{
    if (str != NULL){
        printf("%s\n", str);
    }

    return str;
}

namespace MYSPACE
{
    typedef const char * (*fun)(const char * str);
    
    //fun f = ::test;
    struct ST{
        ST()
        {
            f = test;
        }
    fun f;
    };

}

int main(int argc, char *argv[])
{
    using namespace MYSPACE;

    ST st;

    st.f("hello world");

    return 0;
}
