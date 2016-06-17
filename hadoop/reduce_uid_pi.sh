#!/bin/awk

BEGIN {
    FS = "\t"
}


{
        a[$0] = $3    

}

END {
    slen = asorti(a, b)
    uid = ""
    str = ""
    for (i = 1; i<= slen; i++){
        split(b[i], arr, "\t")
        if (uid == arr[1]) {
            str = sprintf("%s\t %s\t", str, arr[3])
        }else {
            uid = arr[1]
	    printf("%s\n", str)
            str = sprintf("%s\t%s\t", uid, arr[3])
        }
    }

    printf("%s\n", str)
}

