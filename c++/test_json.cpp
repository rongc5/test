#include <iostream>
#include <string>
#include <stdio.h>

using namespace std;

int main(int argc, char *argv[])
{
    string str;

   str = "{\"num\":99, \"time\":123456789}";
    
   printf("%s\n", str.c_str());

   return 0;
}
