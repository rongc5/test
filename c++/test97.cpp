#include <iostream>

using namespace std;

class B;
class A
{
	A(int c):a(c){}
	private:
		int a;
		friend class B;
};

class B:public A
{
	
	public:
		B(int c):A(c){}
		void display();
};

void B::display()
{
	cout << a << '\n';
}

int main()
{
	B b(99);
	
	b.display();
	
	return 0;
}
