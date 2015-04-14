#include <iostream>
#include <string>
#include <map>

using namespace std;

class A{
    public:
        A(){b = new map<int, string>;}
        ~A(){delete b; cout << "A is delete!"<<endl;}
        void insert(int m, string n){b->insert(std::pair<int,string>(m,n));}
        //int show(){cout << "b: " << b->c_str() << endl; return 0;}
        int show();
        /*void show(){B::show();}*/ //error
    private:
        A(const A &);
        A &operator = (const A &);
        int a;
        map< int, string > *b;
};
int A::show()
{
    map< int, string >::iterator it;
    for (it = b->begin(); it != b->end(); ++it)
        cout << it->first << it->second.c_str() << endl;
    cout<<"hello" << endl;

    return 0;
}

int main(int argc, char *argv[])
{
    int b= 6;
   
    A a;

    a.insert(1, "zm");
    a.insert(2, "yy");

    a.show();

    A c(a);

    return 0;
}
