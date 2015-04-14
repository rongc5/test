#include <iostream>
#include <list>

using namespace std;

class A
{
	public:
	A()
	{
		cout << "constuct A" << '\n';
	}
	
	~A()
	{
		cout << "distcontruct A" << '\n';
	}
};



int main()
{
	A *a = new A();
	
	list<A *> mylist;
	
	mylist.push_back(a);
	
	mylist.pop_front();
	
	return 0;
}