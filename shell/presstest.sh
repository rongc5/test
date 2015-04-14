#!/bin/bash

echo "请输入启动程序数量: "
read proc_num

if [ -d "tmp" ];then
	 rm -rf tmp
fi

mkdir tmp

#path=/home/haier/haier_upass1.5_presstest
path=$(pwd)

cp ./haier_client ./tmp
cd ./tmp

truncate -s 0 $path/tmp/log.ok
truncate -s 0 $path/tmp/log.err


for (( i=0 ; i < $proc_num; i++ ))
do 
	./haier_client 1>/dev/null 2>&1
done

while [ "1" = "1" ]
do 
	ret=$(ps aux | grep -v "grep" | grep haier_client | wc -l)
	if [ $ret -eq 0 ]
	then break
	fi
	sleep 3
done


cat $path/tmp/*.ok >> log 

mv log log.ok 

cat $path/tmp/*.err >> log 
mv log log.err
ls $path/tmp/*.ok | grep -v "log.ok" |  xargs rm -rf 
ls $path/tmp/*.err | grep -v "log.err" |  xargs rm -rf 


retok=$(cat log.ok | wc -l)
reterr=$(cat log.err | wc -l)
mintime=$(cat log.ok  | cut -d ":" -f 2 | cut -d " " -f 3 | sort -n | head -1)
maxtime=$(cat log.ok  | cut -d ":" -f 2 | cut -d " " -f 3 | sort -n | tail -1)

sum=$((retok+reterr))
echo "all num is $sum"
echo "the ok num is $retok" 
echo "and the min time is $mintime"
echo "and the max time is $maxtime"

cat log.ok | cut -d ":" -f 2 | cut -d " " -f 3 | sort -n | awk 'BEGIN{total = 0;len = 0} {if($1~/^[0-9]+\.[0-9]*/){total += $1; len++}} END{print"and the ave time is " total/len}'

echo "the err num is $reterr"

cd ../
