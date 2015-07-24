#!/bin/awk -f

BEGIN {
        FS=" "
	ds_ret_baidu = 0
	asp_ret_baidu = 0
	asp_req_baidu = 0
	baidu_ret_success = 0
	pv = 0
}

{
	flag = 0;
        for (i = 1; i <= NF; i++)
        {
		if ($i =="pd=3")
		    flag += 1
		   #printf("%s\n", $i)
		if ($i == "pt=6"){
		    flag += 1
		   #printf("%s\n", $i)
		}

		if (2 == flag && match($i, "cmatch_ad_src_type"))
		{
			if (match($i, "261,11") || match($i, "261,12") ||  match($i, "261,13"))
			{
				asp_ret_baidu += 1
				#printf("%s ", $i)
			}		

		}

                if (2 == flag && match($i, "third_ad_info") && match($i, ","))
                {
			asp_req_baidu += 1
			split($i, a, ",")
			if (a[3] > 0)
				baidu_ret_success += 1
			#printf("%s \n", $i);
                }

		if (2 == flag && match($i, "ds_ret_src_no_list") && match($i, "261:9"))
		{
			ds_ret_baidu += 1;
			#printf("%s ", $i);
			
		}
        }
	if (flag == 2) {
		pv += 1
		#printf("\n");
	}
}

END {
	if (0 != pv)
		printf("pv: %d ds ret: %.6f req baidu:%.6f baidu ret:%.6f, asp ret baidu:%.6f\n", 
			pv, ds_ret_baidu*1.0/pv, asp_req_baidu*1.0/pv, baidu_ret_success *1.0/pv, asp_ret_baidu*1.0/pv)
}
