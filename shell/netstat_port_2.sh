#!/bin/sh
i=0
echo "已建立连结队列里套接字的数量"
while [ "1" = "1" ]
do  
    netstat -nat | grep ESTABLISHED | grep 7881 | wc -l
    ret=$?
    if [ $ret -gt 1 ] && [ $i -eq 0 ]
    then $ret >> ./tmp; date >> ./tmp
        i=$((i+1))
    elif [ $ret -gt 1 ]&&[ $i -gt 0 ];then 
        echo $ret >> ./tmp 
        date >> ./tmp
    elif [ $ret -eq 0 ] && [ $i -gt 0 ];then 
        echo $ret >> ./tmp; date >> ./tmp
    elif [ $ret -eq 0 ] && [ $i -eq 0 ];then
        continue
    fi
    sleep 2
done
