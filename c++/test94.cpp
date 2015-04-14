#include <iostream>
#include <string>
#include <vector>

using namespace std;

class A
{
    public:
        virtual void* run() = 0;
        void display()
        {
            cout << "hello world!!!" << '\n';
        }
        virtual void start()
        {
            _thread_vec.push_back(this);
        }

    private:
        static vector<A*> _thread_vec;
};


int main(int argc, char *argv[])
{
    


    return 0;
}
