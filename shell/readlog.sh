#!/bin/bash

i=1
val=10

Pathlog=/home/haier/Modify_utv_upass1.5_src_epoll/log/accept.log
Pathtmp=/home/haier/Modify_utv_upass1.5_src_epoll/log/tmp
Pathtmp1=/home/haier/Modify_utv_upass1.5_src_epoll/log/tmp1
Presslog=/home/haier/Modify_utv_upass1.5_src_epoll/log/press.log

touch $Pathlog
touch $Pathtmp
touch $Pathtmp1



while [ "1" = "1" ]
do
ret=$(ps ajx | grep haier_server | grep -v "grep" | wc -l)
if [ $ret -eq 0 ]
then
    sleep 5
    echo "程序未启动" >> $Pathlog
    echo -e "\n" >> $Pathlog
    i=$((i+1))
    if [ $i -gt 2 ]
        then exit
     fi
fi
    break
done

i=1
while [ "1" = "1" ]
do
truncate -s 0 $Pathtmp
tail -$val $Presslog | tac > $Pathtmp
ret=$(diff $Pathtmp $Pathtmp1 | wc -l)
if [ $ret -eq 0 ]
then       
        if [ $i -gt 2 ]
                then exit
        fi
        i=$((i+1))
        continue
fi
        break
done

i=1

while [ "1" = "1" ]
do
ret=$(cat $Pathtmp | grep accept | wc -l)
if [ $ret -eq 0 ]
then
        if [ $i -gt 3 ]
                then exit
        fi
        i=$((i+1))
        tail -$val $Presslog | tac > $Pathtmp
        continue
fi
    break
done

truncate -s 0 $Pathtmp1
echo $Pathtmp > $Pathtmp1

cat $Pathtmp | while read line
do
    ret=$(echo $line | grep accept | wc -l)
    if [ $ret -eq 1 ]
    then
        echo -e "\n" >> $Pathlog
        echo $line >> $Pathlog
        echo $(ipcs -q) >> $Pathlog
        exit
    fi
done
