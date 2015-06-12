#include <iostream>
#include <string>
#include <map>

using namespace std;

class A{
    public:
        A(){}
        ~A(){cout << "A is delete!"<<endl;}
        void insert(int m, string n){m_map.insert(std::pair<int,string>(m,n));}
        //int show(){cout << "b: " << b->c_str() << endl; return 0;}
        virtual int show();
        /*void show(){B::show();}*/ //error
    private:
        A(const A &);
        A &operator = (const A &);
        map< int, string > m_map;
};

int A::show()
{
    map< int, string >::iterator it;
    for (it = m_map.begin(); it != m_map.end(); ++it)
        cout << it->first << it->second.c_str() << endl;
    //cout<<"hello" << endl;

    return 0;
}

int main(int argc, char *argv[])
{
    int b= 6;
   
    A a;

    a.insert(1, "zm");
    a.insert(2, "yy");

    a.show();

    return 0;
}
