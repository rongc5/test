#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

int myprintf(void *arg, ...)
{
	char * tmp = NULL;
	va_list ap;
	va_start(ap, arg); 
	tmp = va_arg(ap, char *);
	printf("first:%s\n", (char *)arg);
	printf("tmp:%s\n", tmp);	
	while ( tmp != "no")
	{	
		tmp = va_arg(ap, char *);
		printf("tmp:%s\n", tmp);	
	}
	va_end(ap);


	return 0;	
}

foo(char *fmt, ...)
{
	va_list ap;
	int d;
	char c, *s;

	va_start(ap, fmt);
	while (*fmt)
		switch (*fmt++) {
			case 's':              /* string */
				s = va_arg(ap, char *);
				printf("string %s\n", s);
				break;
			case 'd':              /* int */
				d = va_arg(ap, int);
				printf("int %d\n", d);
				break;
			case 'c':              /* char */
				/* need a cast here since va_arg only
				 *                       takes fully promoted types */
				c = (char) va_arg(ap, int);
				printf("char %c\n", c);
				break;
		}
	va_end(ap);
}






int main(void)
{
	char buf[10] = "hello";
	char buf1[10] = "haier";

	printf("buf: %s buf1: %s\n", buf, buf1);
	myprintf(buf, buf1, "no");
	//foo("%s, %s\n", buf, buf1);

	return 0;
}
