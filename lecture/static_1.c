#include <stdio.h>

//static 修饰局部变量
int f()
{
	static i = 1;
	++i;
	int j = 0;
	j++;
	return i;
}

int main()
{
	for (int i = 0; i < 3; i++)
		printf("%d\n", f());
		
	return 0;
}


int f()
{
	static i = 1;
	++i;

	return 0;
}

int main()
{
	
	printf("%d\n", i);
		
	return 0;
}


static i = 1;
int g()
{
	++i;

	return 0;
}

int main()
{
	
	printf("%d\n", i);
		
	return 0;
}

int b[10] = {1,2, 3, 4, 6};
const int * ptr = b;
*ptr = 10;(error)
ptr++;
*ptr= 10;(正确)
