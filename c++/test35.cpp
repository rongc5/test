#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    try{
        throw 20;
    }catch (int e){
        cout << e << std::endl;
    }

    return 0;
}
