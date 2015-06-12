#include <iostream>
#include <list>

using namespace std;

class TE{
    public:
    TE():channelId(0), programid(0), pCount(0){}
    TE(int a, int b, int c):channelId(a), programid(b), pCount(c){
//        cout << "channelId " << channelId << "programid" << programid << "pCount" << pCount << endl;
    }
    void display(){
        cout << "channelId " << channelId << "programid" << programid << "pCount" << pCount << endl;
    }
    private:
    int channelId;
    int programid;
    int pCount;
};

int main()
{
    cout << endl;
    int len = 0 ;

    list< class TE > test;
    cout << "sizeof(test.front())" << sizeof(test.front()) << test.size() << endl;
    TE qq(1, 2, 3);
    TE mm(4, 5, 6);
    test.push_back(qq);
    test.push_back(mm);

    for (int i = 0; i < test.size(); i++){
        cout << "sizeof(test.front())" << sizeof(test.front()) << test.front().display() << endl;
        //len + = 
    }

    cout << endl;

    return 0;
}
