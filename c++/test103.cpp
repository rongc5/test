#include <iostream>

using namespace std;

class A
{
	public:
	A()
	{
		cout << "constuct A" << '\n';
	}
	
	virtual ~A()
	{
		cout << "distcontruct A" << '\n';
	}
};

class B: public A
{
	public:
	B()
	{
		cout << "constuct B" << '\n';
	}
	
	virtual ~B()
	{
		cout << "distcontruct B" << '\n';
	}
};

int main()
{
	A * pa = new B();
	
	delete pa;
	
	return 0;
}