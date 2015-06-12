#!/bin/awk -f
#=============================================================================
#     FileName: awk_cpm.sh
#         Desc:  
#       Author: zm
#        Email: 
#     HomePage: 
#      Version: 1.0.0
#   LastChange: 2015-05-26 12:05:22
#      History:
#=============================================================================
#
#先通过log_parser.py 将需要的字段过滤出来
#bid, qvalue
#
#


BEGIN {
    FS = " "
}

{
    #if (!(match($0, "cmatch#") || match($0, "qvalue#") || match($0, "bid#")))
        #return

    for (i = 1; i <= NF; i++) 
        {
            if (match($i, "cmatch#") && match($i, "qvalue#"))
            {
                split($i, c, "#")
		#printf("length ==> %d\n", length(c))
                for (k = 2; k <= length(c); k = k+ 3)
                    {
                        tmp = int(k/4)
                        cmatch[tmp] = int(c[2 + tmp*3])
                        bid[tmp] = int(c[3 + tmp*3])
                        qvalue[tmp] = int(c[4 + tmp*3])
                    }

            }
            else if (match($i, "wvar="))
            {
                     if (2 == split($i, t, "="))
                     wvar = t[2]
                     #printf("hello : %s\n", wvar)
            }
            else if (match($i, "cmatch_ad_src_type="))
            {
                split($i,  cmatch_src_list, "=")
                len = split(cmatch_src_list[2], cmatch_ads, ";")
                for (k = 1; k <= len; k++)
                {
                    if (split(cmatch_ads[k], cmatch_type, ",") == 2)
                    {
                        for (m = 0; m < length(cmatch); m++)
                            {
                                if (cmatch[m] == int(cmatch_type[1]))
                                    {
                                        type[m] = int(cmatch_type[2])
                                    }
                            }
                    }
                }
            }


        }


        for (i = 0; i < length(cmatch); i++)
        {
                #printf("%s, %s\n", cmatchs[i], wvar);
            printf("%d  %s  %s  %d, %d, %.6f\n", cmatch[i], wvar, type[i], bid[i], qvalue[i], qvalue[i] * bid[i]/1000000.0)
        }

        delete cmatch
}


