#include <fstream>
#include <iostream>

using namespace std;

int main(void)
{
	
	ofstream fout;    
	                  
	fout.open("11");
	if (fout.is_open())
	{
		fout << "hello world" << " :" << '\n';
		fout << "zhang ming" << " :" << '\n';
	}  
    
    fout.close();
    
	return 0;    
}