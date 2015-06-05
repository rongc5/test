#!/bin/awk -f

BEGIN {
	FS = " "
}

{
	for (i = 1; i <= NF; i++)
	{
		if (match($i, "ds_cnt="))
		{
			split($i, array, "=")
			#printf("%s\n", array[2])
			split(array[2], cmatch_src, ";")
			for (j = 1; j < length(cmatch_src); j++)
			{
				printf("%s\n", cmatch_src[j]);
			}
		}
	}
}
