#include <iostream>
#include <vector>
#include <string>

using namespace std;

class A
{
    public:
        virtual void operate()=0;
        virtual void add(A *){}
        virtual void del(A *){}
        virtual A * get(int index){}
        virtual ~A(){}
};

class leaf: public A
{
    public:
        leaf(const char * str):name(str){}
        virtual void operate()
        {
            cout << "I am leaf: " << name.c_str() << '\n';
        }
    private:
        string name;
};

class composite: public A
{
    public: 
        virtual void operate()
        {
           for (ITERATOR it = _mgr.begin(); it != _mgr.end(); ++it)
           {
               (*it)->operate();
           }
            
        }
        virtual void add(A * a)
        {
            _mgr.push_back(a);
        }

        virtual void del(A * a)
        {
           for (ITERATOR it = _mgr.begin(); it != _mgr.end(); ++it)
               if (*it == a)
               {
                   _mgr.erase(it);
                   break;
               }
        }
        virtual ~composite()
        {
            for (int ii = 0; ii < _mgr.size(); ii++)
            {
                A * ptmp = _mgr.back();
                delete ptmp;
                _mgr.pop_back();
            }
        }
    
    private:
        vector<A *> _mgr;
        typedef  vector<A *>::iterator ITERATOR;

};

int main(int argc, char *argv[])
{
   A * root = new composite;

   A * leaf1 = new leaf("z");
   A * leaf2 = new leaf("h");
   root->add(leaf1);
   root->add(leaf2);
   
   root->operate();

   delete root;

    return 0;
}
