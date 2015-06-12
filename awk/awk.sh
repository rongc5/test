#!/bin/awk -f

##
#
# 在notice log 中查看cmatch_ad_src_type ， 是否请求了某个广告源
#
#
# author: zm 2015年5月12日14:04:25
#

BEGIN {
	FS=" "
}

{
	for (i = 1; i < NF; i++)
	{
		if (match($i, "cmatch_ad_src_type") && split($i, a, "=") == 2)
		{
			if (split(a[2], b, ",") == 2 && match(b[2], "8"))
			{		
				printf("%s\n", $0);
				break;
			}
		}
	}
}
