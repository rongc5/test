#include <iostream>
#include <string>

using namespace std;

#define N 100
int a[N];
int count;

int  comb2(int m,int k)//(C(m,k))
{    
    int i,j;
    for (i=m;i>=k;i--)
    {
        a[k]=i;
        if (k>1)
        {
            comb2(i-1,k-1);
        }
        else
        { 
            count++;
            cout << "hello world "<< a[0] << k << endl;
            //for (j=a[0];j>0;j--)
            for (j=2;j>0;j--)
            {
                cout<<a[j];
            }
            cout<<",";

        }
    }

    return count;           
}

int main()
{
    int m,k;
    cin>>m>>k;
    a[0]=k;
    int num=comb2(m,k);
    cout<<endl;
    cout<<num<<endl;
    return 0;
}
