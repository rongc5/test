#include <iostream>
#include <vector>

using namespace std;

int main()
{
    vector<int>vi;
    int a;

    while (true)
    {
        cout<<"输入一个整数, 按0停止输入:";
        cin>>a;

        if (!a)
            break;

        vi.push_back(a);
        vector<int>::iterator iter;
        for (iter=vi.begin(); iter != vi.end(); ++iter)
            cout<<*iter;
    }

    return 0;
}
