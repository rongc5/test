#include <iostream>
#include <string.h>

using namespace std;

int main(int argc, char *argv[])
{
    int a = 5;
    int b = 6;

    float c = 7.5;
    double d = 9.9;
     //const type_info& t1 = 

    cout << "****" << typeid(a).name() << "~~~" << typeid(c).name() <<"!!!!" << typeid(d).name()<< endl;
    if (typeid(a).name() == typeid(b).name())
    {
        cout << "==>" << typeid(a).name() << endl;
    }

    return 0;
}
