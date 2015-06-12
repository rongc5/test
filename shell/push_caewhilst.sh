#!/bin/bash


pushpath=/home/haier/unitServer/log/CAEWhilst/logback/
backpath=/home/zm/log/CAEWhilst
cd $pushpath
echo "[`date`] name crode [$name_corde]">>../namecrode_push.txt

for file in $pushpath
do
	echo "[$file] backup <$i> date:[`date`]">>$pushpath/namecrode_push.txt
	#scp -P 2022 -r $pushpath/$ls_name root@119.57.11.34:/opt/LOG_server130/pushlog
	rsync -avz $file '-e ssh -p 2022' root@119.57.151.50:$backpath
	if [ $? -eq 0 ];then
		echo "[ok]--[return $?]----$file">>../namecrode_push.txt
		rm -f $file
	else
		echo "[err]--[return $?]---$file">>../namecrode_push.txt
	fi
done
