#!/bin/bash

i=1
val=10
ntime=0
touch accept.log

while [ "1" = "1" ]
do

ret=$(ps ajx | grep haier_server | grep -v "grep" | wc -l)
if [ $ret -eq 0 ]
then
    sleep 3
    continue
fi

if [ $i -gt 1 ]
then echo -e "\n\n\n\n\n\n" >> accept.log
fi

prefix="第$i种算法启用  start at $(date)"
printf "$prefix\t OK\n" >> accept.log

while [ "1" = "1" ]
do
    ret=$(ps ajx | grep haier_server | grep -v "grep" | wc -l)
    if [ $ret -eq 0 ]
    then  break
    fi
    truncate -s 0 tmp
    tail -$val press.log | tac > tmp
    ret=$(cat tmp | grep accept | wc -l)
    if [ $ret -eq 0 ]
        then continue
    fi
    cat tmp | while read line
do
    ret=$(echo $line | grep accept | wc -l)
    if [ $ret -eq 1 ]
    then
        echo -e "\n\n" >> accept.log
        echo $line >> accept.log
        echo $(ipcs -q) >> accept.log
        break
    fi
done
sleep 60
done
i=$((i+1))
done

