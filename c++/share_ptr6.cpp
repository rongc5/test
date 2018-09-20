#include <iostream>
#include <memory>
#include <string>
#include <map>


using namespace std;

class FOO{

    public :
        FOO()
        {
            _name = "def";
            cout << _name << ":FOO()" <<'\n';
        }

        FOO(const char * name):_name(name)
        {
            
            cout << _name << ":FOO()" <<'\n';
        }

        ~FOO()
        {
            cout << _name << ":~FOO()\n";
        }
    private:
        std::string _name;
};

    
    shared_ptr<FOO> foo(new FOO());
int main(int argc, char *argv[])
{
    map<int, shared_ptr<FOO> > mymap;
    {

        shared_ptr<FOO> f1(new FOO("A"));
        mymap[1] = f1;
        cout << "3 Here the refcount is " << f1.use_count() << endl;
        shared_ptr<FOO> f2(new FOO("B"));
        mymap[1] = f2;
        cout << "3 Here the refcount is " << f2.use_count() << endl;
        cout << "3 Here the refcount is " << f1.use_count() << endl;
    }

    {
        map<int, shared_ptr<FOO> > tmp;
        mymap.swap(tmp);
    cout << "So here it will go out of scope and reach 1" << endl;
    }
    
    cout << "So here it will go out of scope and reach 2" << endl;
    //cout << "3 Here the refcount is " << mymap[1].use_count() << endl;

    cout << "So here it will go out of scope and reach 3" << endl;
    return 0;
}
