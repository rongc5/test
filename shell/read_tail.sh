#!/bin/bash


#tail -10 press.log | tac | while read line

val=10
list="accept"

findstr()
{
    str1="$list"
    str2="$1"
    len=${#str1}

    for((i=$len;i>0;i--))
    do
        k=$((len-i))
        for((j=0;j<=$k;j++))
        do
            if echo "$str2" | grep -qF "${str1:$j:$i}"
            then
                ret=${str1:$j:$i}
                echo "max str is '${str1:$j:$i}'"
                if [ "$ret" = $str1 ]
                    then return 0
            fi
        fi
    done
    return 1;
done
}

    while [ "1" = "1" ]
    do 
        tail -$val press.log | tac | cat | while read line 
    do
        #expr index "$line" "$list"
        #echo ${}

        findstr "$line"	
        ret=$?
        if [ $ret -eq 0 ]
        then  date >> accept.log
            echo $line >> accept.log
            break
        fi
    done
    sleep 10
done


