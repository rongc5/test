#include <iostream>
#include <string>
#include <map>

using namespace std;

class A{
    public:
        A(){cout << "hello world" << '\n';}
        ~A(){cout << "A is delete!"<<endl;}
        void insert(int m, string n){m_map.insert(std::pair<int,string>(m,n));}
        //int show(){cout << "b: " << b->c_str() << endl; return 0;}
        /*void show(){B::show();}*/ //error
        virtual void release(){delete this;}
        void size(){cout << m_map.size() << '\n';}
    private:
        virtual int show()
        {
            map< int, string >::iterator it;
            for (it = m_map.begin(); it != m_map.end(); ++it)
                cout << it->first << it->second.c_str() << endl;

            return 0;
        }
        A(const A &);
        A &operator = (const A &);
        map< int, string > m_map;
};


int main(int argc, char *argv[])
{
    int b= 6;
   
    A * ptr = new A;
    ptr->size();
    ptr->insert(1, "zm");
    ptr->insert(2, "yy");
    //ptr->show();
    ptr->release();
    cout << "12345\n" << endl;

    return 0;
}
