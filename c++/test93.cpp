#include <iostream>
#include <string>
#include <stdio.h>
//#include <stdlib.h>

using namespace std;

class A
{
    public:
        A(){}
        A(string & tmp_str):str(tmp_str){printf("%s\n", str.c_str());}
        A(const A & tmp){str = tmp.str; printf("copy:%s\n", tmp.str.c_str());}
        A & operator=(const A & tmp)
        {
            str = tmp.str;
            printf("==: %s\n", tmp.str.c_str());
        }
    private:
        string str;
};

int main(int argc, char *argv[])
{
    string str("hello world");

    //A b=a;

    //b = a;

    try 
    {
        A a(str);
        throw a;
    }
    catch (A )
    {
        //printf("%s\n", b.str)
        ;
    }

    return 0;
}
