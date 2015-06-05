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

BEGIN {
    FS = " "
}

{
    for (i = 1; i <= NF; i++) 
        {
            if (match($i, "wvar"))
            {
                   if (split($i, b, "=") == 2)
                       {
                           #printf("%s\n", $i);
                           wvar = b[2]
                       }
            }
            else if (match($i, "cmatch_ad_src_type"))
            {
                     #printf("hello : %s\n", $i)
                     split($i, cmatch_src, "=")    
                     len = split(cmatch_src[2], cmatchs, ";") 
            }

            if (i == 99) 
            {
                bid = int($i)
            }
            else if (i == 107)
            {
                cmatch = $i
                #printf("%s\n", $i)
            }
            else if (i == 115)
            {
                qvalue = int($i)
                #printf("%s\n", $i)
            }

        }


        for (i = 1; i <= len; i++)
            {
                #printf("%s, %s\n", cmatchs[i], wvar);
                if (split(cmatchs[i], cmatch_ads, ",") == 2)
                    {
                        if (cmatch == cmatch_ads[1])
                            {
                                printf("%s  %s  %s  %d\n", cmatch, cmatch_ads[2], wvar, qvalue * bid)
                            }
                    }

            }
}


