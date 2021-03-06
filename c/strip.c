#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

enum _trim_direction_mode  
{  
    E_TRIM_MODE_NONE = 0x0,  
    E_TRIM_MODE_FRONT = 0x1,  
    E_TRIM_MODE_BACK = 0x2,  
    E_TRIM_MODE_ALL = 0x3,  
};  
typedef enum _trim_direction_mode trim_direction_mode_t;  


static void trim(char *source_string, const char *trim_string, trim_direction_mode_t trim_mode)
{	
	int input_len = strlen(source_string);
	int trim_string_len = strlen(trim_string);
	char *input_value_ptr = source_string;
	const char *trim_string_ptr = trim_string; 

	int escape_flag = 0;
	int i = 0, j = 0;

	assert(source_string != NULL);
	assert(trim_string != NULL);

	if((trim_mode & E_TRIM_MODE_FRONT) != 0)
	{
		/* delete the front character */
		for (i = 0; i < input_len; i++)
		{
			/* cycle the trim string, if the character isn't one of trim string, 
			 * escape the cycle, and move the string */
			for (j = 0; j < trim_string_len; j++)
			{
				if (input_value_ptr[i] == trim_string_ptr[j])
				{
					break;
				}
				if(j == (trim_string_len - 1))
				{
					escape_flag = 1;
				}
			}	
			if(escape_flag)
			{
				break;
			}
		}	
		memmove(input_value_ptr, input_value_ptr+i,(input_len - i + 1)); 
	}	

	if((trim_mode & E_TRIM_MODE_BACK) != 0)
	{
		input_len = strlen(input_value_ptr);
		escape_flag = 0;
		/* delete the end character */
		for (i = input_len - 1; i >= 0; i--)
		{
			/* cycle the trim string, if the character isn't one of trim string, 
			 * escape the cycle, and move the string */
			for (j = 0; j < trim_string_len; j++)
			{
				if (input_value_ptr[i] == trim_string_ptr[j])
				{
					break;
				}
				if(j == (trim_string_len - 1))
				{
					escape_flag = 1;
				}
			}	
			if(escape_flag)
			{
				break;
			}
		}	
		input_value_ptr[i+1] = '\0';
	
	}

	return;
}


int main(int argc, char *argv[])
{
    char buf[512]="hhheeeeeelllhhhh";

    printf("%s\n", buf);
    trim(buf, "hhh", E_TRIM_MODE_ALL);

    printf("%s\n", buf);

    return 0;
}
