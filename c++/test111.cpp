#include <iostream>
#include <string>
#include <list>

using namespace std;

class item
{
    public:
        void display()
        {
            cout << "id: " << id << " str: " << str.c_str() << '\n';
        }
        void set(int a, string & src)
        {
            id = a;
            str = src;
        }
        
    private:
        int id;
        string str;
};

template< class T1, class T2=list<T1> >
class A
{
    public:
        A()
        {
            _list = new T2();
        }
        ~A()
        {
            delete _list;
        }

        void push(T1 & node)
        {
            _list->push_back(node);
        }

        void display()
        {
            typedef typename T2::iterator itl;
            for (itl it = _list->begin(); it != _list->end(); ++it)
                it->display();
        }
    private:
        T2 * _list;
};


int main(int argc, char *argv[])
{
    A< item> a;
    
    item node;
    for (int i = 0; i < 10; i++)
    {
        string str("hello");
        node.set(i, str);
        a.push(node);
    }

    a.display();

    return 0;
}
