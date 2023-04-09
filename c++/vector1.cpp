#include <iostream>
#include <vector>

using namespace std;

int main()
{
    vector<int>vi;
    int a;

    vi.push_back(123);
    for (int i = 0; i < vi.size(); i++) {
        vi.push_back(9);
        cout << vi.size() << endl;
    }

    return 0;
}
