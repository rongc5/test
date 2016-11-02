#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isDigit(unsigned char ch)
{
	if(ch>= '0' && ch <= '9')
	{
		return 1;
	}
	else
	{
		return 0;
	}	
}


unsigned char  character_to_hex(unsigned char highByte, unsigned char lowByte)
{
	highByte = isDigit(highByte)? (highByte - '0'):(highByte - 'A' + 10);
	lowByte = isDigit(lowByte)? (lowByte - '0'):(lowByte - 'A' + 10);
	return ((highByte<<4) + lowByte);
}



unsigned char* urldecode(unsigned char* srcBuf, int srcLen, int *outLen)
{
	if(NULL == srcBuf || srcLen<= 0)
	{
		return NULL;
	}

	unsigned char* dstBuf = NULL;
	dstBuf = (unsigned char *)malloc(srcLen);
	if(NULL == dstBuf)
	{
		return NULL;
	}
	
	int i=0, j=0;
	for(i=0; i<srcLen; i++)
	{
		if(srcBuf[i] == '%')
		{
			dstBuf[j] = character_to_hex(srcBuf[i+1], srcBuf[i+2]);
			i+=2;
		}
		else if(srcBuf[i] == '+')
		{
			dstBuf[j] = ' ';
		}
		else
		{
			dstBuf[j] = srcBuf[i];
		}
			
		j++;
	}

	*outLen = j;
	return dstBuf;
}


int main(int argc, char *argv[])
{
	
	char * str = "<!DOCTYPE HTML><meta charset='utf-8'><style type='text/css'>html,body{ width:100%; height:100%;}* { padding: 0px; margin: 0px;}a:link { text-decoration: none;}</style><a href=";
	int size = 0;
	unsigned char* dst = urldecode(str, strlen(str), &size);
	printf("%s\n", dst);	

	return 0;
}

