#include <stdio.h>
#include <stdlib.h>

#define SHR(a, b)       \
  (-1 >> 1 == -1        \
   ? (a) >> (b)         \
   : (a) / (1 << (b)) - ((a) % (1 << (b)) < 0))

int main(void)
{
	printf("%d, %d\n", -1, -1>>1);

	printf("%d\n", (-1) / (1 << (1)) - ((-1) % (1 << (1)) < 0));
	
	return 0;
}
