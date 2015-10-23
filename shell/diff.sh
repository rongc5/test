#!/bin/awk

BEGIN {
    dst=2.txt
}

{
    for (i = 1; i<=NF; i++)
    system("grep $i  $dst | wc -l") | getline line

       if (int(line)) {
        printf("%s\n", $i)
    }
}

END {

}
