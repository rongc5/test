#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    int array[] = {2,2,3,4,5};

    for (auto &i:array)
        cout << "==> " << i << '\n';
    
    return 0;
}
