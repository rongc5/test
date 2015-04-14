#include <iostream>
#include <list>

using namespace std;

class TE{
    public:
    TE():channelId(0), programid(0), pCount(0){}
    TE(int a, int b, int c):channelId(a), programid(b), pCount(c){
        cout << "channelId " << channelId << "programid" << programid << "pCount" << pCount << endl;
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
    list<char> coll;

    for (char c = 'a'; c <= 'z'; ++c){
        coll.push_back(c);
    }

    while (! coll.empty()){
        cout << coll.front() << ' ';
        coll.pop_front();
    }

    cout << endl;

    list< class TE > test;
    TE qq(1, 2, 3);
    test.push_back(qq);

    test.front().display();

    cout << endl;

    return 0;
}
