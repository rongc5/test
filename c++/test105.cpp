#include <iostream>

using namespace std;

void fun(bool a=false)
{
    if (a)
        cout << "true" << '\n';
    else
        cout << "false" << '\n';
}

int main(int argc, char *argv[])
{
    fun(true);

    return 0;
}
