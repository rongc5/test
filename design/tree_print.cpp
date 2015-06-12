#include <iostream>
#include <set>
#include <string>

using namespace std;

class base
{
    public:
        virtual void show() = 0;
        virtual void add(base *){}
        virtual ~base(){}
};

class node: public base
{
    public:
        node(int index):id(index){}

        virtual void show()
        {
            for (int i = 0; i < id; i++)
                cout << " ";
            cout << id << " ";
        }

        virtual ~node(){}
    private:
        int id;
};

class tree:public base
{
    public:
        tree(int index):id(index){}

        virtual void add(base * b)
        {
            _mgr.insert(b);
        }

        virtual void show()
        {
            for (int i = 0; i < id; i++)
                cout << " ";
            cout << id <<'\n';

            for (ITER it = _mgr.begin(); it != _mgr.end(); ++it)
                (*it)->show();

            cout << '\n';
        }

        virtual ~tree()
        {
            for (ITER it = _mgr.begin(); it != _mgr.end(); ++it)
            {
                _mgr.erase(it);
                delete *it;
            }
        }

    private:
        set<base *> _mgr;
        typedef set<base *>::iterator ITER;
        int id;
};


int main(int argc, char *argv[])
{

    base * ptree = new tree(0);
    for (int ii = 1; ii <6; ii += 2)
    {
        base * root = new tree( 6 - ii);

        base * leaf1 = new node(6 - ii -1);
        base * leaf2 = new node(6 - ii - 1);
        
        root->add(leaf1);
        root->add(leaf2);

        ptree->add(root);
    }

    ptree->show();


    return 0;
}
