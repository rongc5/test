#include <iostream>
#include <string>
#include <vector>

using namespace std;

class orginer;
class memento
{
    private:
        memento(string & st):_st(st){}

        string & get_state()
        {
            return _st;
        }

        void set_state(string & st)
        {
            _st = st;
        }
    private:
        friend class orginer;
        string _st;
};

class orginer
{
    public:
        orginer(){_st = "helllo world";}
        orginer(const char * str):_st(str){}
        memento * save()
        {
            return new memento(_st);
        }

        void set(const char * str)
        {
            _st = str;
        }

        void recovery(memento * mem)
        {
            if (!mem)
                return;
            _st = mem->get_state();
            delete mem;
        }

        void print()
        {
            cout << _st << '\n';
        }

    private:
        string _st;
};

class memger
{
    public:
        void set(memento * mem)
        {
            _mgr.push_back(mem);
        }
        memento * get()
        {
           if (!_mgr.size())
               return NULL;
           memento * ptr =  _mgr.back();
            _mgr.pop_back();
           return ptr;
        }
    private:
        typedef vector<memento *>::iterator ITER;
        vector<memento *> _mgr;
};

int main(int argc, char *argv[])
{
    orginer *por = new orginer("bilin");    
    //por->print();

    memger * p_mgr = new memger;
    p_mgr->set(por->save());
    
    por->set("qq");
    p_mgr->set(por->save());

    por->set("zm");
    p_mgr->set(por->save());


    por->recovery(p_mgr->get());
    por->print();

    por->recovery(p_mgr->get());
    por->print();

    por->recovery(p_mgr->get());
    por->print();

    return 0;
}
