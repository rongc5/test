#include <iostream>
#include <string.h>
#include <string>
#include <list>
#include <map>

using namespace std;

int a = 99;

class B{
    public:
        void show(){cout << "hello"<<endl;cout << a << endl;}
};



class A{
    public:
        A(const int & a, const string &b):a(a), b(b){ cout << "a:" << b << endl;this->show();}
        ~A(){}
        void show(){cout << "a: " << a <<" "<< "b: " << b << endl;}
        /*void show(){B::show();}*/ //error
    private:
        int a;
        string b;
};




int main(int argc, char *argv[])
{
    //int b= 6;
    //cout << "b:" << &b << endl;
    //A a(b, "hello");
    B b;
    b.show();
   
    //a.show();

    //string b("hello");

   //cout << b.c_str() << endl;
   //b.append("world");
   //cout << b.c_str() << endl;

   //b = string("zhangming");
   //cout << b.c_str() << endl;
   //b="qianfeng";
   //cout << b.c_str() << endl;

    //map< int, ,list<string> > mymap;
    //list<string> lr;
    //lr.push_back("hello");
    //lr.push_back("world");
    //lr.push_back("qianfeng");
    
    //list<string> mm;
    
    //list<string>::iterator it;
    //for (it = lr.begin(); it != lr.end(); ++it){
            //cout << it->c_str() << endl;
            //mm.push_back(*it);
            //cout<<"hello" << endl;
    //}
    
    //mm.push_back("zhang");
    //mm.push_back("ming");
    //for (it = mm.begin(); it != mm.end(); ++it)
            //cout << it->c_str() << endl;
    
    return 0;
}
