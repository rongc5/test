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


211     c       9       42.6661190184   0.9930461802
211     t       9       39.1470581429   0.9966741129
228     c       9       44.6263239947   0.9988404581
228     t       9       61.8949949819   0.9993094740
229     c       9       36.1001554489   0.9597158532
229     t       9       60.4120979090   0.9768001490
230     c       9       36.1001554489   0.9597158532
230     t       9       60.4120979090   0.9768001490
240     c       9       45.0061353119   0.9502871844
251     c       9       21.6245258983   0.9838352427
261     c       9       19.0976788078   0.9925368526
261     t       9       64.4793000213   0.2000000000
291     c       9       15.8148965291   0.9955050829
310     t       9       143.7761106780  1.0
598     t       9       229.0199276668  0.9996838702
620     t       9       88.8619226539   1.0

awk -F',' '{for (i = 1; i <= NF; i++) if (match($i, "\"charge\":1")) arr[{]++} END{for (i in arr) print i,arr[i]}'}

grep 'pd=29 pt=2' asp.log.nt* | grep '07-23' | awk -F' ' '{for (i = 1; i <= NF; i++) if (index($i, "mid=") && !index($i, "mmid=")) print $i}' |wc -l
grep 'appid=10001' access.log.20190919* | awk -F' ' '{print [}' | awk -F'&' '{for (i=1; i <= NF; i++){if (match($i, "userInitPay=")) arr[
$i]++}} END {for (i in arr) print i,arr[i]}']
