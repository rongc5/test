#include <iostream>

using namespace std;

class A
{
    public:
        A(int c):a(c){}
    protected:
        void show()
        {
            cout << "hello world" << '\n';
        }
    protected:
		int a;
};

class B:private A
{
	public:
        B(int c):A(c){}
        void display();
};

void B::display()
{
    cout << a << '\n';
    show();
}

int main()
{
	B b(99);
	
	b.display();
	
	return 0;
}
