#include <iostream>
#include <string>


using namespace std;

class A{
    public:
    explicit A(int a);
    int function(const A &a) const
    {
        cout << "function()" << "a.x" << a.x <<endl;
    }
    ~A();
    private:
    int x;
};

A::A(int a){
    cout << "construct " << endl; 
    x = a;
}

A::~A(){
    cout << "descontruct" << endl;
}


int main(void)
{
    A a(3);
    
    a.function(a);

    return 0;
}
