#include <iostream>
#include <string.h>
#include <string>
#include <list>
#include <map>

using namespace std;

//class B{
    //public:
        //void show(){cout << "hello"<<endl;}
//};



//class A{
    //public:
        //A(const int & a, const string &b):a(a), b(b){ cout << "a:" << b << endl;this->show();}
        //~A(){}
        //void show(){cout << "a: " << a <<" "<< "b: " << b << endl;}
        //[>void show(){B::show();}<] //error
    //private:
        //int a;
        //string b;
//};




int main(int argc, char *argv[])
{
    //int b= 6;
    //cout << "b:" << &b << endl;
    //A a(b, "hello");
   
    //a.show();

    //string b("hello");

   //cout << b.c_str() << endl;
   //b.append("world");
   //cout << b.c_str() << endl;

   //b = string("zhangming");
   //cout << b.c_str() << endl;
   //b="qianfeng";
   //cout << b.c_str() << endl;

    map< int, list<string> > mymap;
    //list<string> &lr = mymap[1];
    //lr.push_back("hello");
    //lr.push_back("world");
    //lr.push_back("qianfeng");
    
    for (int i = 1; i < 10; i++){
        list<string> &lr = mymap[i];
        string str = "hello";
        lr.push_back(str);
        /*cout << lr.c_str() << endl;  */
    }

    //list<string>::iterator it = mymap[1].begin();
    //std::cout << mymap[1] << '\n';
    

    //return 1;
    
    map< int, list<string> >::iterator it;
    for (it = mymap.begin(); it != mymap.end(); ++it)
            cout << ":" << it->first << *(it->second.begin()) << endl;
            cout<<"hello" << endl;
    
    
        

    return 0;
}
