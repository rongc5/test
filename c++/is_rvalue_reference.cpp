#include <type_traits>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    int i = 6;
    int arr[5] = {0};
    int *ptr = arr;

    cout << is_rvalue_reference<decltype(arr)>::value << endl;

    return 0;
}
