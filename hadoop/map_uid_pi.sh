#!/bin/awk
BEGIN {
	FS = " "
}

{
    if (match($0, /[0-9][0-9]-[0-9][0-9] [0-9][0-9]:[0-9][0-9]:[0-9][0-9]/, m)){
        time = m[0]
    }


    for (i = 1; i<= NF; i++) {
        split($i, a, "=")
        if (a[1] == "uid")
            uid = a[2]
        else if (a[1] == "pi")
            pi = a[2]
    }

    printf("%s\t%s\t%s\n", uid, time, pi)
}

